#include <movement/LowLevelMotorControl.h>
#include <movement/RoboModule.h>
#include <movement/RoboMovementAlgorithm.h>
#include <movement/RoboMovementControl.h>
#include <movement/RoboMovementSensor.h>
#include "Timers.h"

/*
 * Internal operation declarations
 */

static CallTree* _roboCallTree; // For what?

// Current state of robo-module (both for menu-interface and robo-program execution flow)
static volatile RoboState _moduleState = 0;
// Last error code
static RoboError _error = 0;

static void executeNextInstruction();
static void executeCurrentInstruction();
static void executionEnd();

// Time in ms to wait
static volatile uint16_t _msToWait;
// Callback for wait-timer.
static volatile CallPtr _onWaitEnd;

// Program that is currently executed. If RoboModule_ExecuteInstrucion() is called program is 0.
static RoboProgram* _currentProgram;
// Instruction that is currently executed
static RoboInstruction* _currentInstruction;

static MovementSensor* _moveSensor;
static MovementAlgorithm* _moveAlgorithm;

void roboWaitUpdate();
void RoboWait(uint16_t time);

static inline int roboIsBusy()
{
	return (_moduleState & RoboState_Busy) > 0;
}

static inline int roboIsIdle()
{
	return (_moduleState == RoboState_Idle);
}

static inline int roboIsStopped()
{
	return (_moduleState == RoboState_Stopped);
}

static inline int roboIsMoving()
{
	return (_moduleState & RoboState_Moving) > 0;
}

static inline int roboIsWaiting()
{
	return (_moduleState & RoboState_Waiting) > 0;
}

static inline int roboIsInsturctionEnded()
{
	return (_moduleState & RoboState_InstuctionEnd) > 0;
}

/*
 * ------------------------------------------------------------------
 * RoboModule interface section
 * ------------------------------------------------------------------
 */
bool RoboModule_IsBusy()
{
	return (bool)((_moduleState & RoboState_Busy) > 0);
}

RoboState RoboModule_GetState()
{
	return _moduleState;
}

RoboError RoboModule_GetError()
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
	_moduleState = RoboState_Stopped;
	_error = RoboError_NoError;

	SetSensorType(SensorType_Software);
	_moveSensor = GetMovementSensor();

	_moveSensor->Init();
}

void RoboModule_Start()
{
	_moduleState = RoboState_Idle;
	MotoResume();
	_moveSensor->StartReadings();
}

void RoboModule_Stop()
{
	_moduleState = RoboState_Stopped;
	_onWaitEnd = 0;
	_moveSensor->StopReadings();
	MotoStandby();
}

void roboMoveUpdate(float timeStep);
void RoboModule_Update(uint16_t interval)
{
	if(roboIsInsturctionEnded())
	{
		if(_currentProgram != 0)
		{
			_moduleState = RoboState_Busy;
			executeNextInstruction();
		}
		else
			_moduleState = RoboState_Idle;
	}
	else if(roboIsMoving())
	{
		// Update with time in 100us
		roboMoveUpdate((float)interval);
	}

	_moveSensor->Update(interval);
}

// Executes all instructions in program
// Saves program to be executed in _currentProgram
bool RoboModule_ExecuteProgram(RoboProgram* program)
{
	if(_moduleState != RoboState_Idle)
	{
		_error = RoboError_Busy;
		return false;
	}

	if(program->instCount == 0)
	{
		_error = RoboError_Exe_EmptyProg;
		return false;
	}

	_currentProgram = program;
	_currentInstruction = _currentProgram->firstInstruction;
	_moduleState = RoboState_Busy;

	executeCurrentInstruction();

	return true;
}

bool RoboModule_ExecuteInstrucion(RoboInstruction* instruction)
{
	if(_moduleState != RoboState_Idle)
	{
		_error = RoboError_Busy;
		return false;
	}

	if(instruction == 0 || instruction->functionDef == 0)
	{
		_error = RoboError_Exe_NullInst;
		return false;
	}

	_moduleState = RoboState_Busy;
	_currentInstruction = instruction;

	executeCurrentInstruction();

	return true;
}

inline void executionEnd()
{
	_moduleState = RoboState_Idle;
}

inline void executeNextInstruction()
{
	if(_currentInstruction == 0) // No more instructions -> end program
	{
		executionEnd();
		return;
	}

	_currentInstruction = _currentInstruction->next;
	executeCurrentInstruction();
}

inline void executeCurrentInstruction()
{
	// It should be assured that current instruction is valid
	SetMovementAlgorithmType(_currentInstruction->functionDef->steering);
	_moveAlgorithm = GetMovementAlgorithm();

	if((_currentInstruction->functionDef->function & RoboFunction_Movement) > 0)
	{
		_moveAlgorithm->SetCurrentInstruction(_currentInstruction);
	}
	else if((_currentInstruction->functionDef->function & RoboFunction_ResetSensors) > 0)
	{
		_moveSensor->ResetOrientation();
		_moveSensor->ResetPosition();
	}
	else if((_currentInstruction->functionDef->function & RoboFunction_Wait) > 0)
	{
		RoboWait(_currentInstruction->params[0]);
	}
}

void roboMoveUpdate(float timeStep)
{
	if(_moveAlgorithm->CheckIsTargetReached())
	{
		executeNextInstruction();
		return;
	}

	float velLin, velAng;
	_moveAlgorithm->GetSteeringForFrame(
			timeStep, &velLin, &velAng);

	RoboSetVelocity(velLin, velAng);
}

void RoboWait(uint16_t time)
{
	_onWaitEnd = executeNextInstruction;
	_msToWait = time;
	_moduleState |= RoboState_Waiting;
	RegisterTickFunction_NOASSERT( roboWaitUpdate, SLOT_RoboWait );
}

void roboWaitUpdate()
{
	if(_msToWait <= 0)
	{
		UnregisterTickFunction(SLOT_RoboWait);
		_moduleState &= ~RoboState_Waiting;
		if(_onWaitEnd != 0)
			_onWaitEnd();
	}
	else
		_msToWait--;
}
