#include "RoboModule.h"
#include "MotorControl.h"
#include "Timers.h"
#include "RoboMove.h"
#include "Encoders.h"

/*
 * Internal operation declarations
 */

static CallTree* _roboCallTree;
static RoboState _moduleState = 0;
static RoboError _error = 0;
static void _executeNextInstruction();
static void _executionEnd();

static volatile bool _isWaiting;
static volatile uint16_t _msToWait;
static volatile CallPtr _onWaitEnd;
static volatile bool _instrEnd; // Indicates end of current instruction

static RoboProgram* _currentProgram;

/*
 * ------------------------------------------------------------------
 * RoboModule interface section
 * ------------------------------------------------------------------
 */
__inline__ bool RoboModule_IsBusy()
{
	return (bool)(_moduleState == Robo_Busy);
}

__inline__ RoboState RoboModule_GetState()
{
	return _moduleState;
}

__inline__ RoboError RoboModule_GetError()
{
	return _error;
}


void RoboModule_Init(Module* module)
{
	module->Start = RoboModule_Start;
	module->Stop = RoboModule_Stop;
	module->Update = RoboModule_Update;

	InitMotorIO();
	CALLTREE_ALLOC(_roboCallTree, 4);
	_roboCallTree->curFun = 0;
	_moduleState = Robo_Stopped;
	_error = Robo_Err_NoError;
}

void RoboModule_Start()
{
	_moduleState = Robo_Idle;
	_instrEnd = false;
	MotoResume();
}

void RoboModule_Stop()
{
	_moduleState = Robo_Stopped;
	_onWaitEnd = 0;
	MotoStandby();
}

void RoboModule_Update(uint16_t interval)
{
	if( _instrEnd)
	{
		// Execution of current instruction has ended
		// so execute new one
		_instrEnd = false;
		if(_currentProgram != 0)
			_executeNextInstruction();
		else
			_moduleState = Robo_Idle;
	}
}

// Executes all instructions in program
// Saves program to be executed in _currentProgram
static RoboProgram* _currentProgram;
bool RoboModule_ExecuteProgram(RoboProgram* program)
{
	if(_moduleState != Robo_Idle)
	{
		_error = Robo_Err_Busy;
		return false;
	}

	if(program->instCount == 0)
	{
		_error = Robo_ExeErr_EmptyProg; // Empty program
		return false;
	}

	_currentProgram = program;
	_currentProgram->currentInstruction = _currentProgram->firstInstruction;
	_moduleState = Robo_Busy;
	_executeNextInstruction();

	return true;
}

// Executes all instructions in program
// Saves program to be executed in _currentProgram

bool RoboModule_ExecuteInstrucion(RoboInstruction* instruction)
{
	if(_moduleState != Robo_Idle)
	{
		_error = Robo_Err_Busy;
		return false;
	}

	if(instruction == 0 || instruction->functionDef == 0)
	{
		_error = Robo_ExeErr_NullInst; // Empty instruction
		return false;
	}

	Encoders_ResetAll();
	Encoders_StartRead();
	_moduleState = Robo_Busy;
	ExecuteRoboFunction(instruction);

	return true;
}

void _roboWaitUpdate()
{
	if(_msToWait <= 0)
	{
		UnregisterTickFunction(SLOT_RoboWait);
		if(_onWaitEnd != 0)
			_onWaitEnd();
	}
	else
		_msToWait--;
}

__inline__ void _roboWait(CallPtr onEnd)
{
	_onWaitEnd = onEnd;
	_msToWait = ROBO_PULSE_LEN;
	_isWaiting = true;
	RegisterTickFunction_NOASSERT( _roboWaitUpdate, SLOT_RoboWait );
}

void _executionEnd()
{
	_moduleState = Robo_Idle;
}

/*
 * ------------------------------------------------------------------------
 * Robo movement instructions execution section
 * ------------------------------------------------------------------------
 */

#define _CURINST _currentProgram->currentInstruction
void _executeNextInstruction()
{
	if(_CURINST == 0) // No more instructions -> end program
	{
		// finalize execution - maybe NotifyExeEnd()
		_executionEnd();
		return;
	}

	RoboInstruction* instruction = _CURINST;
	_CURINST = _CURINST->next;

	Encoders_ResetAll();
	Encoders_StartRead();
	// Execute the actual function with saved params
	ExecuteRoboFunction(instruction);
}

/*
 *  Movement common function/variables declarations
 */

// Pulse queue variables
// In movement successive pulses are send to motors
static uint8_t _pulsesCount = 0;
static uint8_t _currentPulse = 0;
static uint8_t _pulseQueueFL[ROBO_PULSE_COUNT_MAX+1];
static uint8_t _pulseQueueFR[ROBO_PULSE_COUNT_MAX+1];
static uint8_t _pulseQueueBL[ROBO_PULSE_COUNT_MAX+1];
static uint8_t _pulseQueueBR[ROBO_PULSE_COUNT_MAX+1];

// Sends next pulse to motors from queue
static void _roboMoveNext()
{
	if(_currentPulse >= _pulsesCount) // End movement
	{
		_instrEnd = true;
		_currentPulse = 0;
		return;
	}

	// Run all motors with next pulses
	MotoMove(_pulseQueueFL[_currentPulse], _pulseQueueFR[_currentPulse],
			_pulseQueueBL[_currentPulse], _pulseQueueBR[_currentPulse]);
	_currentPulse++;

	// wait for next pulse -> then send another one
	_onWaitEnd = _roboMoveNext;
	_msToWait = ROBO_PULSE_LEN;
	_isWaiting = true;
	RegisterTickFunction_NOASSERT( _roboWaitUpdate, SLOT_RoboWait );
}

/*
 * Specific movements functions
 */

// Regulated robo move functions:
// - in function set distance and speed to be made by motors
// - when getting new pulse read encoders
// - have set distance to travel D0
// - distance made so far = Dact
// - error in distance = D0-Dact = Drem
// - velocity based on last 10 motopulse updates
// - V = dist_in_10pulses / pulse_lenght
// - set velocity: V0, error: eV = V0 - V
// - regulation in distance give only maximum speed and its sign
// ( if robo happen to ride over D0 )
// - regulation of speed keeps speed on level = max(V0, limit on D reg)
//
// So, max velocity = K * eD, where K is such that in last 2-3 pulses
// velocity began to fall down
// Velocity is regulated as PD :
// - start with expected V0 ( pulse = V0/maxV ) : Td = 1
// - kp ~= 9 (90%), Ti - such that after 1 motopulse, v should ?
// steering signal should not be set velocity but expected distance
// in time : so while moving in line: Dexp ~ t * speed for every motor
// when moving on circle those expected distances will vary depending
// on radius
// If dist > speed * dist_in_10_pulses :
// 		start expected distance regulation after 10 pulses
//		for first 10 pulses move with set speed
// else
//		move with set speed and regulate only absolute distance

// 1) Only distance regulation -> go 100% till d < d0
// When d - d0 <= 4 enc pulse * speed/100, go with 66%
// speed, when d - d0 <= 2 enc pulse go with 33%

static float _moveDistances[4];
static float _currentDistance[4];
static uint8_t _currentPulses[4];
static float _moveSpeeds[4];

__inline__ uint8_t _roboGetTargetPulse(uint8_t moto)
{
	// When distance is travelled - stop
	// If rem dist <= 2*enc_pulse -> move with 33%
	// If rem dist <= 4*enc_pulse -> move with 66%
	// Else move full speed
	if(_currentDistance[moto] >= _moveDistances[moto])
		return 0;
	else if(_currentDistance[moto] >= _moveDistances[moto] - 2 * ENC_DIST_PER_PULSE )
		return ROBO_PULSE_LEN * 0.33f * _moveSpeeds[moto];
	else if(_currentDistance[moto] >= _moveDistances[moto] - 4 * ENC_DIST_PER_PULSE )
		return ROBO_PULSE_LEN * 0.66f * _moveSpeeds[moto];
	else
		return ROBO_PULSE_LEN * _moveSpeeds[moto];
}

__inline__ float _roboGetTargetVelocity(uint8_t moto)
{
	// When distance is travelled - stop
	// If rem dist <= 2*enc_pulse -> move with 33%
	// If rem dist <= 4*enc_pulse -> move with 66%
	// Else move full speed
	if(_currentDistance[moto] >= _moveDistances[moto])
		return 0;
	else if(_currentDistance[moto] >= _moveDistances[moto] - 2 * ENC_DIST_PER_PULSE )
		return ROBO_VEL_MAX * 0.33f * _moveSpeeds[moto];
	else if(_currentDistance[moto] >= _moveDistances[moto] - 4 * ENC_DIST_PER_PULSE )
		return ROBO_VEL_MAX * 0.66f * _moveSpeeds[moto];
	else
		return ROBO_VEL_MAX * _moveSpeeds[moto];
}

void _roboMoveDistance()
{
	uint8_t moto;
	for(moto = 0; moto < 4; moto++)
	{
		// Get current distance and proper next pulse length
		_currentDistance[moto] = Encoders_GetDistance(moto);

		_currentPulses[moto] = _roboGetTargetPulse(moto);

		// If any motor finished distance, assume all did
		if(_currentPulses[moto] == 0)
		{
			// For now backward regulation disabled, so end
			_instrEnd = true;
			Encoders_StopRead();
			return;
		}
	}

	// Run all motors with next pulses
	MotoMove(_currentPulses[ENC_FL], _currentPulses[ENC_FR],
			_currentPulses[ENC_BL], _currentPulses[ENC_BR]);

	// wait for next pulse -> then send another one
	_roboWait(_roboMoveDistance);
}

#define _ave_mpul_count 12
static float _curVel[4];
static float _distChanges[_ave_mpul_count][4];
static float _velError[4];
static uint8_t _steerPulse[4];
static float _velErrInt[4]; // integral part of eV

void _roboRegulateConstantVelocity();
// First n pulses just register readings
static uint8_t countdown = _ave_mpul_count - 1;
void _roboRegulateConstatntVelocityPrepare()
{
	if(countdown <= 0)
	{
		_roboRegulateConstantVelocity();
	}
	else
	{
		uint8_t moto = 0;
		countdown--;
		for(; moto < 4; moto++)
		{
			_distChanges[countdown][moto] = Encoders_GetDistance(moto);
			_steerPulse[moto] = (uint8_t)(1.0f * _moveSpeeds[moto] * ROBO_PULSE_LEN);
		}
		MotoMove(_steerPulse[ENC_FL], _steerPulse[ENC_FR],
					_steerPulse[ENC_BL], _steerPulse[ENC_BR]);

		_roboWait(_roboRegulateConstatntVelocityPrepare);
	}
}

void _roboRegulateConstantVelocity()
{
	uint8_t moto, i;
	for(moto = 0; moto < 4; moto++)
	{
		if(Encoders_GetDistance(moto) >= _moveDistances[moto])
		{
			_instrEnd = true;
			Encoders_StopRead();
			return;
		}
		float targetVel = _roboGetTargetVelocity(moto);
		// Shift table
		for(i = _ave_mpul_count - 1; i > 0; i--)
			_distChanges[i][moto] = _distChanges[i-1][moto];
		// Get freshest distances
		_distChanges[0][moto] = Encoders_GetDistance(moto);
		// Get new current velocity [mm/s]
		_curVel[moto] = ((_distChanges[0][moto] - _distChanges[_ave_mpul_count-1][moto]) // path
				* 1000 ) / (ROBO_PULSE_LEN * _ave_mpul_count); // divided by time

		// Now get eV
		_velError[moto] = targetVel - _curVel[moto];
		// Just modify the steering by adding 0.5 of error value to _steerPulse
		// Steer relative : pul_len*curVel/steerPul = velPerPulse
		float steer;
		if(_curVel[moto] > 1)
			steer = (float)_steerPulse[moto] * (1 + 0.1f * _velError[moto] / _curVel[moto]);
		else
			steer = (float)_steerPulse[moto] + 0.1f *_velError[moto] * ROBO_PULSE_LEN / ROBO_VEL_MAX;

		if(steer < 0)
			_steerPulse[moto] = 0;
		else if(steer > ROBO_PULSE_LEN)
			_steerPulse[moto] = ROBO_PULSE_LEN;
		else
			_steerPulse[moto] = (uint8_t)steer;
	}

	// Run all motors with next pulses
	MotoMove(_steerPulse[ENC_FL], _steerPulse[ENC_FR],
			_steerPulse[ENC_BL], _steerPulse[ENC_BR]);
	_roboWait(_roboRegulateConstantVelocity);
}

// Dist in millimetres, speed in percents
void RoboMoveFwd(uint16_t dist, uint16_t speed, uint16_t no)
{
	MotoAllForward();

	uint8_t moto;
	for(moto = 0; moto < 4; moto++ )
	{
//		_currentDistance[moto] = 0;
		_moveDistances[moto] = dist;
		_moveSpeeds[moto] = (float)speed / 100.0f;

		_curVel[moto] = 0.0f;
		_steerPulse[moto] = 0;
		_velError[moto]  = 0.0f;
		_velErrInt[moto] = 0.0f;
	}

	countdown = _ave_mpul_count - 1;
	_roboRegulateConstatntVelocityPrepare();
}

void RoboMoveBwd(uint16_t dist, uint16_t speed, uint16_t no)
{
	MotoAllBackward();

	uint8_t moto;
	for(moto = 0; moto < 4; moto++ )
	{
//		_currentDistance[moto] = 0;
		_moveDistances[moto] = dist;
		_moveSpeeds[moto] = (float)speed / 100.0f;

		_curVel[moto] = 0.0f;
		_steerPulse[moto] = 0;
		_velError[moto]  = 0.0f;
		_velErrInt[moto] = 0.0f;
	}

	countdown = _ave_mpul_count - 1;
	_roboRegulateConstatntVelocityPrepare();
}

void RoboWait(uint16_t time, uint16_t x, uint16_t xx)
{
	_onWaitEnd = _executeNextInstruction;
	_msToWait = time;
	_isWaiting = true;
	RegisterTickFunction_NOASSERT( _roboWaitUpdate, SLOT_RoboWait );
}

void RoboRotateLeft(uint16_t angle, uint16_t speed, uint16_t x)
{
	// Angular speed (as) = R/a * wheel_speed_difference / 2
	// Dist to rotate by theta -> v(wheel) / w(robo) = a -> lin_dist(wheel) / ang(robo) = a
	// lin_dist = a * ang = a * pi * ang / 180 [mm]

	// To rotate left -> right motors forward, left backward
	MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
	MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
	MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);

	uint8_t moto;
	for(moto = 0; moto < 4; moto++ )
	{
		_moveDistances[moto] = ROBO_WHEELDIST * PI * (float)angle / 180.0f;
		_moveSpeeds[moto] = (float)speed / 100.0f;

		_curVel[moto] = 0.0f;
		_steerPulse[moto] = 0;
		_velError[moto]  = 0.0f;
		_velErrInt[moto] = 0.0f;
	}

	countdown = _ave_mpul_count - 1;
	_roboRegulateConstatntVelocityPrepare();
}

void RoboRotateRight(uint16_t angle, uint16_t speed, uint16_t x)
{
	// Angular speed (as) = R/a * wheel_speed * speed / 100
	// Ang in single pulse = as * PULSE_LEN / 1000
	// Pulses count = angle / as
	float pulsesCount = (float)angle /
			(((float)ROBO_WHEELRAD/(float)ROBO_WHEELBASE) * ROBO_DEGS_MAX *((float)speed / 100.0)
					* ((float)ROBO_PULSE_LEN / 1000 ));
	// Integral part of pulse count
	uint16_t fullPulsesCount = (uint16_t)pulsesCount;
	// Fractional part - length of last pulse ( 1ms precision )
	uint8_t lastPulseLenght = (uint8_t)((pulsesCount - (float)fullPulsesCount)*speed);

	// If last pulse > 0 then send it, otherwise send only full ones
	_pulsesCount = lastPulseLenght > 0 ? fullPulsesCount + 1 : fullPulsesCount;

	// Fill the pulse queues
	//FILL_PQUEUE(speed, fullPulsesCount, lastPulseLenght);

	// To rotate left -> right motors forward, left backward
	MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
	MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
	MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);

	// Start sending pulses to motors from queue
	_roboMoveNext();
}

void RoboArcFwdLeft(uint16_t radius, uint16_t arclen, uint16_t speed)
{
	// To move on circle : v = wR -> ru1 / r/a u2R ->
	// u1 / u2 = R/a -> fir = u1+u2 = u1(1+r/a),
	// fil = u1-u2 = u2(1-r/a) -> fir/fil=(a+r)/(a-r)
	// pul_len(right) = pul_len_max * speed / 100
	// pul_len(left) = pul_len(right) * (a-r)/(a+r)
	float pul_len_right = (float)ROBO_PULSE_LEN * (float)speed / 100.0;
	float pul_len_left = pul_len_right * (-(float)ROBO_WHEELBASE+radius)/((float)ROBO_WHEELBASE+radius);

	// angle in single pulse = angspeed * pul_len_max / 1000
	// angspeed = 0.5*(pul_len_right-pul_len_left)*degs*r/a
	// pulse count = angle/ang_in_pul
	float pulsesCount = (float)arclen /
			((0.5*(pul_len_right-pul_len_left)/ROBO_PULSE_LEN)*ROBO_DEGS_MAX*((float)ROBO_WHEELRAD/(float)ROBO_WHEELBASE)
					* ((float)ROBO_PULSE_LEN / 1000 ));
	// Integral part of pulse count
	uint16_t fullPulsesCount = (uint16_t)pulsesCount;
	// Fractional part - length of last pulse ( 1ms precision )
	uint8_t lastPulseLenght_L = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_left/ROBO_PULSE_LEN);
	uint8_t lastPulseLenght_R = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_right/ROBO_PULSE_LEN);

	// If last pulse > 0 then send it, otherwise send only full ones
	_pulsesCount = fullPulsesCount + 1;

	MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
	MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	if(pul_len_left < 0)
	{
		pul_len_left = -pul_len_left;
		MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}
	else
	{
		MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}

	// Fill the pulse queues
	int pul;
	for(pul = 0; pul < fullPulsesCount; pul++)
	{
		_pulseQueueFL[pul] = pul_len_left;
		_pulseQueueFR[pul] = pul_len_right;
		_pulseQueueBL[pul] = pul_len_left;
		_pulseQueueBR[pul] = pul_len_right;
	}
	_pulseQueueFL[pul] = lastPulseLenght_L;
	_pulseQueueFR[pul] = lastPulseLenght_R;
	_pulseQueueBL[pul] = lastPulseLenght_L;
	_pulseQueueBR[pul] = lastPulseLenght_R;

	// Start sending pulses to motors from queue
	_roboMoveNext();
}

void RoboArcFwdRight(uint16_t radius, uint16_t arclen, uint16_t speed)
{
	// To move on circle : v = wR -> ru1 / r/a u2R ->
	// u1 / u2 = R/a -> fil = u1+u2 = u1(1+r/a),
	// fir = u1-u2 = u2(1-r/a) -> fil/fir=(a+r)/(a-r)
	// pul_len(left) = pul_len_max * speed / 100
	// pul_len(right) = pul_len(left) * (a-r)/(a+r)
	float pul_len_left = (float)ROBO_PULSE_LEN * (float)speed / 100.0;
	float pul_len_right = pul_len_left * (-(float)ROBO_WHEELBASE+radius)/((float)ROBO_WHEELBASE+radius);

	// angle in single pulse = angspeed * pul_len_max / 1000
	// angspeed = 0.5*(pul_len_left-pul_len_right)*degs*r/a
	// pulse count = angle/ang_in_pul
	float pulsesCount = (float)arclen /
			((0.5*(pul_len_left-pul_len_right)/ROBO_PULSE_LEN)*ROBO_DEGS_MAX*((float)ROBO_WHEELRAD/(float)ROBO_WHEELBASE)
					* ((float)ROBO_PULSE_LEN / 1000 ));
	// Integral part of pulse count
	uint16_t fullPulsesCount = (uint16_t)pulsesCount;
	// Fractional part - length of last pulse ( 1ms precision )
	uint8_t lastPulseLenght_L = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_left/ROBO_PULSE_LEN);
	uint8_t lastPulseLenght_R = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_right/ROBO_PULSE_LEN);

	// If last pulse > 0 then send it, otherwise send only full ones
	_pulsesCount = fullPulsesCount + 1;

	MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
	MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	if(pul_len_right < 0)
	{
		pul_len_right = -pul_len_right;
		MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}
	else
	{
		MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}

	// Fill the pulse queues
	int pul;
	for(pul = 0; pul < fullPulsesCount; pul++)
	{
		_pulseQueueFL[pul] = pul_len_left;
		_pulseQueueFR[pul] = pul_len_right;
		_pulseQueueBL[pul] = pul_len_left;
		_pulseQueueBR[pul] = pul_len_right;
	}
	_pulseQueueFL[pul] = lastPulseLenght_L;
	_pulseQueueFR[pul] = lastPulseLenght_R;
	_pulseQueueBL[pul] = lastPulseLenght_L;
	_pulseQueueBR[pul] = lastPulseLenght_R;

	// Start sending pulses to motors from queue
	_roboMoveNext();
}

void RoboArcBwdLeft(uint16_t radius, uint16_t arclen, uint16_t speed)
{
	// To move on circle : v = wR -> ru1 / r/a u2R ->
	// u1 / u2 = R/a -> fir = u1+u2 = u1(1+r/a),
	// fil = u1-u2 = u2(1-r/a) -> fir/fil=(a+r)/(a-r)
	// pul_len(right) = pul_len_max * speed / 100
	// pul_len(left) = pul_len(right) * (a-r)/(a+r)
	float pul_len_right = (float)ROBO_PULSE_LEN * (float)speed / 100.0;
	float pul_len_left = pul_len_right * (-(float)ROBO_WHEELBASE+radius)/((float)ROBO_WHEELBASE+radius);

	// angle in single pulse = angspeed * pul_len_max / 1000
	// angspeed = 0.5*(pul_len_right-pul_len_left)*degs*r/a
	// pulse count = angle/ang_in_pul
	float pulsesCount = (float)arclen /
			((0.5*(pul_len_right-pul_len_left)/ROBO_PULSE_LEN)*ROBO_DEGS_MAX*((float)ROBO_WHEELRAD/(float)ROBO_WHEELBASE)
					* ((float)ROBO_PULSE_LEN / 1000 ));
	// Integral part of pulse count
	uint16_t fullPulsesCount = (uint16_t)pulsesCount;
	// Fractional part - length of last pulse ( 1ms precision )
	uint8_t lastPulseLenght_L = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_left/ROBO_PULSE_LEN);
	uint8_t lastPulseLenght_R = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_right/ROBO_PULSE_LEN);

	// If last pulse > 0 then send it, otherwise send only full ones
	_pulsesCount = fullPulsesCount + 1;

	MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
	MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	if(pul_len_left < 0)
	{
		pul_len_left = -pul_len_left;
		MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}
	else
	{
		MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}

	// Fill the pulse queues
	int pul;
	for(pul = 0; pul < fullPulsesCount; pul++)
	{
		_pulseQueueFL[pul] = pul_len_left;
		_pulseQueueFR[pul] = pul_len_right;
		_pulseQueueBL[pul] = pul_len_left;
		_pulseQueueBR[pul] = pul_len_right;
	}
	_pulseQueueFL[pul] = lastPulseLenght_L;
	_pulseQueueFR[pul] = lastPulseLenght_R;
	_pulseQueueBL[pul] = lastPulseLenght_L;
	_pulseQueueBR[pul] = lastPulseLenght_R;

	// Start sending pulses to motors from queue
	_roboMoveNext();
}

void RoboArcBwdRight(uint16_t radius, uint16_t arclen, uint16_t speed)
{
	// To move on circle : v = wR -> ru1 / r/a u2R ->
	// u1 / u2 = R/a -> fil = u1+u2 = u1(1+a/r),
	// fir = u1-u2 = u2(1-r/a) -> fil/fir=(a+r)/(-a+r)
	// pul_len(left) = pul_len_max * speed / 100
	// pul_len(right) = pul_len(left) * (r-a)/(r+a)
	float pul_len_left = (float)ROBO_PULSE_LEN * (float)speed / 100.0;
	float pul_len_right = pul_len_left * (-(float)ROBO_WHEELBASE+radius)/((float)ROBO_WHEELBASE+radius);

	// angle in single pulse = angspeed * pul_len_max / 1000
	// angspeed = 0.5*(pul_len_left-pul_len_right)*degs*r/a
	// pulse count = angle/ang_in_pul
	float pulsesCount = (float)arclen /
			((0.5*(pul_len_left-pul_len_right)/ROBO_PULSE_LEN)*ROBO_DEGS_MAX*((float)ROBO_WHEELRAD/(float)ROBO_WHEELBASE)
					* ((float)ROBO_PULSE_LEN / 1000 ));
	// Integral part of pulse count
	uint16_t fullPulsesCount = (uint16_t)pulsesCount;
	// Fractional part - length of last pulse ( 1ms precision )
	uint8_t lastPulseLenght_L = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_left/ROBO_PULSE_LEN);
	uint8_t lastPulseLenght_R = (uint8_t)((pulsesCount - (float)fullPulsesCount)*pul_len_right/ROBO_PULSE_LEN);

	// If last pulse > 0 then send it, otherwise send only full ones
	_pulsesCount = fullPulsesCount + 1;

	MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
	MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	if(pul_len_right < 0)
	{
		pul_len_right = -pul_len_right;
		MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}
	else
	{
		MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}

	// Fill the pulse queues
	int pul;
	for(pul = 0; pul < fullPulsesCount; pul++)
	{
		_pulseQueueFL[pul] = pul_len_left;
		_pulseQueueFR[pul] = pul_len_right;
		_pulseQueueBL[pul] = pul_len_left;
		_pulseQueueBR[pul] = pul_len_right;
	}
	_pulseQueueFL[pul] = lastPulseLenght_L;
	_pulseQueueFR[pul] = lastPulseLenght_R;
	_pulseQueueBL[pul] = lastPulseLenght_L;
	_pulseQueueBR[pul] = lastPulseLenght_R;

	// Start sending pulses to motors from queue
	_roboMoveNext();
}
