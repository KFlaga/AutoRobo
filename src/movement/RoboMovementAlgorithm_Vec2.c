/*
 * RoboMovementAlgorithm_Vec2.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Kamil
 */

#include <movement/RoboMovementAlgorithm.h>
#include <movement/LowLevelMotorControl.h>

#ifdef VECTOR2_MOVEMENT

static MovementAlgorithm movementAlgorithm;
static MovementSensor* _moveSensor;

/*
 * Movement targets queue
 */
#define TARGETS_MAX 4
struct TargetData
{
	Vector2 targetPoint[TARGETS_MAX];
	Vector2 targetDirection[TARGETS_MAX];
	float maxVelocityRel;
	float maxVelocityLinearAbs;
	float maxVelocityAngularAbs;
	uint8_t currentTarget;
	uint8_t targetCount;
	bool haveTargetDirection;
};
static struct TargetData _targetData;

static const float _distanceErrorSquared = 100.f; // allow 1 cm error
static const float _directionErrorSquared = 0.001f;

static void fastestToPoint_GetSteering(float us100, float* velLin, float* velAng);
static void fastestToPoint_SetInstruction(RoboInstruction* instruction);
static bool fastestToPoint_CheckTargetReached();

static void rotateAndGo_GetSteering(float us100, float* velLin, float* velAng);
static void rotateAndGo_SetInstruction(RoboInstruction* instruction);
static bool rotateAndGo_CheckTargetReached();

void SetMovementAlgorithmType(enum MovementAlgorithmType type)
{
	switch(type)
	{
	case MovementAlgorithmType_FastestToPoint:
		movementAlgorithm.SetCurrentInstruction = fastestToPoint_SetInstruction;
		movementAlgorithm.GetSteeringForFrame = fastestToPoint_GetSteering;
		movementAlgorithm.CheckIsTargetReached = fastestToPoint_CheckTargetReached;
	case MovementAlgorithmType_RotateAndGo:
	default:
		movementAlgorithm.SetCurrentInstruction = rotateAndGo_SetInstruction;
		movementAlgorithm.GetSteeringForFrame = rotateAndGo_GetSteering;
		movementAlgorithm.CheckIsTargetReached = rotateAndGo_CheckTargetReached;
		break;
	}

	_moveSensor = GetMovementSensor();
}

MovementAlgorithm* GetMovementAlgorithm()
{
	return &movementAlgorithm;
}

/*
 * ALGORITHM: fastest to point
 */

static void fastestToPoint_SetInstruction_MoveLine(RoboInstruction* instruction);
static void fastestToPoint_SetInstruction_MoveArc(RoboInstruction* instruction);
static void fastestToPoint_SetInstruction_MoveToPoint(RoboInstruction* instruction);
static void fastestToPoint_SetInstruction_Rotate(RoboInstruction* instruction);

static void fastestToPoint_SetInstruction(RoboInstruction* instruction)
{
	_moveSensor = GetMovementSensor();
	switch(instruction->functionDef->function)
	{
	case RoboFunction_MoveLine:
		fastestToPoint_SetInstruction_MoveLine(instruction);
		break;
	case RoboFunction_MoveArc:
		fastestToPoint_SetInstruction_MoveArc(instruction);
		break;
	case RoboFunction_MoveToPoint:
		fastestToPoint_SetInstruction_MoveToPoint(instruction);
		break;
	case RoboFunction_Rotate:
		fastestToPoint_SetInstruction_Rotate(instruction);
		break;
	}
	_targetData.currentTarget = 0;
}

static bool fastestToPoint_CheckTargetReached()
{
	return (_targetData.currentTarget == (_targetData.targetCount-1)) && // Final target
			// Check if reached target point
			(Vector2_DistanceSquared(
					_targetData.targetPoint[_targetData.currentTarget],
					_moveSensor->data.position) < _distanceErrorSquared) &&
			// Check if needs to reach target direction
			(_targetData.haveTargetDirection == false ||
			// Check if reached target direction if it needs to
			Vector2_DistanceSquared(
					_targetData.targetDirection[_targetData.currentTarget],
					_moveSensor->data.direction) < _directionErrorSquared);
}

static inline void fastestToPoint_SetInstruction_MoveLine(RoboInstruction* instruction)
{
	// Params:
	// 1) distance [mm]
	// 2) max velocity [%]
	float distance = (float)instruction->params[0];
	_targetData.maxVelocityRel = 0.01f * (float)instruction->params[1];
	_targetData.maxVelocityLinearAbs = _targetData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	// 1) Just move forward
	_targetData.targetPoint[0] = Vector2_FromXY(
			_moveSensor->data.position.x + _moveSensor->data.direction.x * distance,
			_moveSensor->data.position.y + _moveSensor->data.direction.y * distance
		);
	_targetData.targetCount = 1;
	_targetData.haveTargetDirection = false;
}

static inline void fastestToPoint_SetInstruction_MoveArc(RoboInstruction* instruction)
{
	// Params:
	// 1) radius [mm]
	// 2) angle [deg]
	// 3) max velocity angular [%]
	float radius = (float)instruction->params[0];
	float angle = DEG_TO_RAD((float)instruction->params[1]);
	_targetData.maxVelocityRel = 0.01f * (float)instruction->params[2];
	_targetData.maxVelocityLinearAbs = _targetData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	_targetData.maxVelocityAngularAbs = _targetData.maxVelocityLinearAbs / (float)ROBO_WHEELBASE;
	// Set 3 points in arc as targets, each after 1/3 of target angle
	// Each point is on arc after 1/3 of angle
	// Let (cx,cy) be arc center, (sx,sy) robo pos, (dx,dy) robo dir, R radius
	// (cx,cy) = (sx,sy) + R*(-dy,dx)
	// r0 = (dy,-dx), r1 = r0 rotated by 'angle', (cx,cy) = (sx,sy) - R*r0
	// Final position should be (px,py) = (cx,cy) + R*r1 = (sx,sy) + R*(-r0+r1)
	// And final direction (dfx,dfy) = (-r1.y,r1.x)
	float angle1 = 0.33f * angle;
	float angle2 = 0.67f * angle;
	Vector2 r0 = Vector2_FromXY(
			_moveSensor->data.direction.y,
			-_moveSensor->data.direction.x
	);
	Vector2 r1 = Vector2_Rotate_XY(r0, angle1);
	Vector2 r2 = Vector2_Rotate_XY(r0, angle2);
	Vector2 r3 = Vector2_Rotate_XY(r0, angle);

	// Set targets for each point
	_targetData.targetDirection[0] = Vector2_FromXY(-r1.y, r1.x);
	_targetData.targetPoint[0] = Vector2_FromXY(
			_moveSensor->data.position.x - (r1.x - r0.x) * radius,
			_moveSensor->data.position.y - (r1.y - r0.y) * radius
	);

	_targetData.targetDirection[1] = Vector2_FromXY(-r2.y, r2.x);
	_targetData.targetPoint[1] = Vector2_FromXY(
			_moveSensor->data.position.x - (r2.x - r0.x) * radius,
			_moveSensor->data.position.y - (r2.y - r0.y) * radius
	);

	_targetData.targetDirection[2] = Vector2_FromXY(-r3.y, r3.x);
	_targetData.targetPoint[2] = Vector2_FromXY(
			_moveSensor->data.position.x - (r3.x - r0.x) * radius,
			_moveSensor->data.position.y - (r3.y - r0.y) * radius
	);

	_targetData.targetCount = 3;
	_targetData.haveTargetDirection = true;
}

static inline void fastestToPoint_SetInstruction_MoveToPoint(RoboInstruction* instruction)
{
	// Params:
	// 1) x [mm]
	// 2) y [mm]
	// 3) max velocity [%]
	float x = (float)instruction->params[0];
	float y = (float)instruction->params[1];
	_targetData.maxVelocityRel = 0.01f * (float)instruction->params[2];
	_targetData.maxVelocityLinearAbs = _targetData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	// Set global (x,y) point as target
	_targetData.targetPoint[0] = Vector2_FromXY(
			x,
			y
	);
	_targetData.haveTargetDirection = false;
	_targetData.targetCount = 1;
}

static inline void fastestToPoint_SetInstruction_Rotate(RoboInstruction* instruction)
{
	// Params:
	// 1) angle [deg]
	// 2) max velocity angular [%]
	float angle = DEG_TO_RAD((float)instruction->params[0]);
	_targetData.maxVelocityRel = 0.01f * (float)instruction->params[1];
	_targetData.maxVelocityLinearAbs = _targetData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	_targetData.maxVelocityAngularAbs = _targetData.maxVelocityLinearAbs / (float)ROBO_WHEELBASE;

	_targetData.targetPoint[0] = _moveSensor->data.position;
	_targetData.targetDirection[0] = Vector2_Rotate_XY(_moveSensor->data.direction, angle);
	_targetData.targetCount = 1;
	_targetData.haveTargetDirection = true;
}

/*
 * TODO: write down steering derivation
 * Movement Target:
 * - get to target point/direction as fast as we can with little care about following path
 * - in each loop iteration we have given :
 * --- T : time since last iteration (this iteration is expected to take similar amount of time)
 * --- s : robot's current position
 * --- d : robots's current direction
 * --- p : target point
 * --- r : target direction
 * - as output we need :
 * --- v : linear velocity (assume constant along whole frame)
 * --- w : angular velocity (assume constant along whole frame)
 *
 * I) Steering without target direction
 *	We try to find optimal [v,w] to reach target point.
 *	Max velocities are bounded by: |v| + |w*h| < Vmax, where:
 *	- h is distance from wheel to robot Y axis (aka WHEELBASE)
 *	- Vmax is maximum linear velocity for robot (we assume that it is same for each wheel and is VEL_MAX)
 *	In each iteration we treat movement as if it was first iteration, that is
 *	no knowledge about previous steps is used and only output for
 *	current step is computed (so we may assume that current time is 0)
 *
 *	Let:
 *	- I{a,b}[f(x)dx] denote integral of f(x) from a to b
 *	- ds be change of position in this frame
 *	- dd be change of direction in this frame
 *	We have:
 *	ds = I{0,T}(v * d(t)dt)
 *
 *	If |v| + |wh| < Vmax for optimal move we do:
 *	1) Calculate w needed to reach target direction
 *	2) Calculate v needed to reach target point
 *
 *	If angle between d and (s - p) is smaller than i.e. 1 degree then let w = 0 for this step
 *	If ||s-p|| is smaller than i.e. 1 cm then let v = 0 for this step
 *	Well those limits should be scaled so that ??
 *
 *	Checking for |v| + |wh| < Vmax should be first step:
 *	- compute angle a(d, (s-p)) and check if a < w*T
 *	- if it is check if a < 1deg (or sth) and let w = a/T or 0
 *	- if |wh| < Vmax then check if ||s-p|| < (Vmax-|wh|)*T ??
 *	- if velocities are within limits then
 *
 *	OK, NEVERMIND MAYBE I'LL GET TO IT SOME OTHER TIME
 */

static void fastestToPoint_GetSteering(float us100, float* velLin, float* velAng)
{

}

/*
 * ALGORITHM: rotate and go
 */
static void rotateAndGo_SetInstruction(RoboInstruction* instruction)
{
	_moveSensor = GetMovementSensor();
	switch(instruction->functionDef->function)
	{
	case RoboFunction_MoveLine:
		fastestToPoint_SetInstruction_MoveLine(instruction);
		break;
	case RoboFunction_MoveArc:
		fastestToPoint_SetInstruction_MoveArc(instruction);
		break;
	case RoboFunction_MoveToPoint:
		fastestToPoint_SetInstruction_MoveToPoint(instruction);
		break;
	case RoboFunction_Rotate:
		fastestToPoint_SetInstruction_Rotate(instruction);
		break;
	}
	_targetData.currentTarget = 0;
}

static bool rotateAndGo_CheckTargetReached()
{
	return (_targetData.currentTarget == (_targetData.targetCount-1)) && // Final target
			// Check if reached target point
			(Vector2_DistanceSquared(
					_targetData.targetPoint[_targetData.currentTarget],
					_moveSensor->data.position) < _distanceErrorSquared) &&
			// Check if needs to reach target direction
			(_targetData.haveTargetDirection == false ||
			// Check if reached target direction if it needs to
			Vector2_DistanceSquared(
					_targetData.targetDirection[_targetData.currentTarget],
					_moveSensor->data.direction) < _directionErrorSquared);
}

/*
 * Movement Target:
 * - get to target point/direction as fast as we can with little care about following path
 * - in each loop iteration we have given :
 * --- T : time since last iteration (this iteration is expected to take similar amount of time)
 * --- s : robot's current position
 * --- d : robots's current direction
 * --- p : target point
 * --- r : target direction
 * - as output we need :
 * --- v : linear velocity (assume constant along whole frame)
 * --- w : angular velocity (assume constant along whole frame)
 *
 *	Max velocities are bounded by: |v| + |w*h| < Vmax, where:
 *	- h is distance from wheel to robot Y axis (aka WHEELBASE)
 *	- Vmax is maximum linear velocity for robot (we assume that it is same for each wheel and is VEL_MAX)
 *
 *	If a, angle between d and (s - p), is larger than i.e. 1 deg, then :
 *	- w = min( a / T, w_max ), v = 0
 *	Maybe if a > w_max * 5T, then scale w down somewhat
 *	If a is small enough:
 *	- v = min( ||s-p|| / T, v_max )
 *	Same down-scaling applies here
 *
 *	Scale will be:
 *	k_ang = max(0.1, min(1.0, a / (5*w_max*T)))
 *	k_lin = max(0.1, min(1.0, L / (5*v_max*T))), L = ||s-p||
 */

#define ERROR_ANGLE 0.01f
#define ERROR_DIST 5.0f

static void rotateAndGo_GetSteering(float us100, float* velLin, float* velAng)
{
	Vector2 localTarget = Vector2_Sub(
			_targetData.targetPoint[_targetData.currentTarget],
			_moveSensor->data.position);

	float remAngle = Vector2_AngleBetween(_moveSensor->data.direction, localTarget);
	float remDist = Vector2_Length(localTarget);

	float ms = us100 * 0.1f;

	if(remAngle > ERROR_ANGLE)
	{
		float wmax = _targetData.maxVelocityAngularAbs;
		float scale = roboMax(0.1f, roboMin(1.0f, remAngle / (5*wmax*ms)));
		float aT = remAngle / ms;
		*velAng = scale * roboMin(aT, wmax);
		*velLin = 0.0f;
	}
	else if(remDist > ERROR_DIST)
	{
		float vmax = _targetData.maxVelocityLinearAbs;
		float scale = roboMax(0.1f, roboMin(1.0f, remDist / (5*vmax*ms)));
		float LT = remDist / ms;
		*velAng = 0.0f;
		*velLin = scale * roboMin(LT, vmax);
	}
	else
	{
		*velLin = 0.0f;
		*velAng = 0.0f;
	}
}

#endif
