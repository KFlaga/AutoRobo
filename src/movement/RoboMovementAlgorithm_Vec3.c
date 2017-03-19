/*
 * RoboMovementAlgorithm.c
 *
 *  Created on: Mar 5, 2017
 *      Author: Kamil
 */

#include <movement/RoboMovementAlgorithm.h>
#include <movement/LowLevelMotorControl.h>

#ifdef VECTOR3_MOVEMENT

static MovementAlgorithm movementAlgorithm;
static MovementSensor* _moveSensor;

/*
 * Movement targets queue
 */
#define TARGETS_MAX 4
struct FastestToPoint_Data
{
	Vector3 targetPoint[TARGETS_MAX];
	Vector3 targetDirection[TARGETS_MAX];
	float maxVelocityRel;
	float maxVelocityLinearAbs;
	float maxVelocityAngularAbs;
	uint8_t currentTarget;
	uint8_t targetCount;
	bool haveTargetDirection;
};
static struct FastestToPoint_Data _ftpData;

static const float _distanceErrorSquared = 100.f; // allow 1 cm error
static const float _directionErrorSquared = 0.001f;

static void fastestToPoint_GetSteering(float us100, float* velLin, float* velAng);
static void fastestToPoint_SetInstruction(RoboInstruction* instruction);
static bool fastestToPoint_CheckTargetReached();

void SetMovementAlgorithmType(enum MovementAlgorithmType type)
{
	switch(type)
	{
	case MovementAlgorithmType_FastestToPoint:
	default:
		movementAlgorithm.SetCurrentInstruction = fastestToPoint_SetInstruction;
		movementAlgorithm.GetSteeringForFrame = fastestToPoint_GetSteering;
		movementAlgorithm.CheckIsTargetReached = fastestToPoint_CheckTargetReached;
		break;
	}

	movementAlgorithm.movementSensor = GetMovementSensor();
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
	_ftpData.currentTarget = 0;
}

static bool fastestToPoint_CheckTargetReached()
{
	return (_ftpData.currentTarget == (_ftpData.targetCount-1)) && // Final target
			// Check if reached target point
			(Vector3_DistanceSquared(
					_ftpData.targetPoint[_ftpData.currentTarget],
					_moveSensor->data.position) < _distanceErrorSquared) &&
			// Check if needs to reach target direction
			(_ftpData.haveTargetDirection == false ||
			// Check if reached target direction if it needs to
			Vector3_DistanceSquared(
					_ftpData.targetDirection[_ftpData.currentTarget],
					_moveSensor->data.direction) < _directionErrorSquared);
}

static inline void fastestToPoint_SetInstruction_MoveLine(RoboInstruction* instruction)
{
	// Params:
	// 1) distance [mm]
	// 2) max velocity [%]
	float distance = (float)instruction->params[0];
	_ftpData.maxVelocityRel = 0.01f * (float)instruction->params[1];
	_ftpData.maxVelocityLinearAbs = _ftpData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	// 1) Just move forward
	_ftpData.targetPoint[0] = Vector3_FromXYZ(
			_moveSensor->data.position.x + _moveSensor->data.direction.x * distance,
			_moveSensor->data.position.y + _moveSensor->data.direction.y * distance,
			_moveSensor->data.position.z
	);
	_ftpData.targetCount = 1;
	_ftpData.haveTargetDirection = false;
}

static inline void fastestToPoint_SetInstruction_MoveArc(RoboInstruction* instruction)
{
	// Params:
	// 1) radius [mm]
	// 2) angle [deg]
	// 3) max velocity angular [%]
	float radius = (float)instruction->params[0];
	float angle = DEG_TO_RAD((float)instruction->params[1]);
	_ftpData.maxVelocityRel = 0.01f * (float)instruction->params[2];
	_ftpData.maxVelocityLinearAbs = _ftpData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	_ftpData.maxVelocityAngularAbs = _ftpData.maxVelocityLinearAbs / (float)ROBO_WHEELBASE;
	// Set 3 points in arc as targets, each after 1/3 of target angle
	// Each point is on arc after 1/3 of angle
	// Let (cx,cy) be arc center, (sx,sy) robo pos, (dx,dy) robo dir, R radius
	// (cx,cy) = (sx,sy) + R*(-dy,dx)
	// r0 = (dy,-dx), r1 = r0 rotated by 'angle', (cx,cy) = (sx,sy) - R*r0
	// Final position should be (px,py) = (cx,cy) + R*r1 = (sx,sy) + R*(-r0+r1)
	// And final direction (dfx,dfy) = (-r1.y,r1.x)
	float angle1 = 0.33f * angle;
	float angle2 = 0.67f * angle;
	Vector3 r0 = Vector3_FromXYZ(
			_moveSensor->data.direction.y,
			-_moveSensor->data.direction.x,
			_moveSensor->data.direction.z
	);
	Vector3 r1 = Vector3_Rotate_XY(r0, angle1);
	Vector3 r2 = Vector3_Rotate_XY(r0, angle2);
	Vector3 r3 = Vector3_Rotate_XY(r0, angle);

	// Set targets for each point
	_ftpData.targetDirection[0] = Vector3_FromXYZ(-r1.y, r1.x, 0.f);
	_ftpData.targetPoint[0] = Vector3_FromXYZ(
			_moveSensor->data.position.x - (r1.x - r0.x) * radius,
			_moveSensor->data.position.y - (r1.y - r0.y) * radius,
			_moveSensor->data.position.z
	);

	_ftpData.targetDirection[1] = Vector3_FromXYZ(-r2.y, r2.x, 0.f);
	_ftpData.targetPoint[1] = Vector3_FromXYZ(
			_moveSensor->data.position.x - (r2.x - r0.x) * radius,
			_moveSensor->data.position.y - (r2.y - r0.y) * radius,
			_moveSensor->data.position.z
	);

	_ftpData.targetDirection[2] = Vector3_FromXYZ(-r3.y, r3.x, 0.f);
	_ftpData.targetPoint[2] = Vector3_FromXYZ(
			_moveSensor->data.position.x - (r3.x - r0.x) * radius,
			_moveSensor->data.position.y - (r3.y - r0.y) * radius,
			_moveSensor->data.position.z
	);

	_ftpData.targetCount = 3;
	_ftpData.haveTargetDirection = true;
}

static inline void fastestToPoint_SetInstruction_MoveToPoint(RoboInstruction* instruction)
{
	// Params:
	// 1) x [mm]
	// 2) y [mm]
	// 3) max velocity [%]
	float x = (float)instruction->params[0];
	float y = (float)instruction->params[1];
	_ftpData.maxVelocityRel = 0.01f * (float)instruction->params[2];
	_ftpData.maxVelocityLinearAbs = _ftpData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	// Set global (x,y) point as target
	_ftpData.targetPoint[0] = Vector3_FromXYZ(
			x,
			y,
			_moveSensor->data.position.z
	);
	_ftpData.haveTargetDirection = false;
	_ftpData.targetCount = 1;
}

static inline void fastestToPoint_SetInstruction_Rotate(RoboInstruction* instruction)
{
	// Params:
	// 1) angle [deg]
	// 2) max velocity angular [%]
	float angle = DEG_TO_RAD((float)instruction->params[0]);
	_ftpData.maxVelocityRel = 0.01f * (float)instruction->params[1];
	_ftpData.maxVelocityLinearAbs = _ftpData.maxVelocityRel * (float)ROBO_VEL_MAX_PER_MS;
	_ftpData.maxVelocityAngularAbs = _ftpData.maxVelocityLinearAbs / (float)ROBO_WHEELBASE;

	_ftpData.targetPoint[0] = _moveSensor->data.position;
	_ftpData.targetDirection[0] = Vector3_Rotate_XY(_moveSensor->data.direction, angle);
	_ftpData.targetCount = 1;
	_ftpData.haveTargetDirection = true;
}

/*
 * TODO: write down steering derivation
 */

static void fastestToPoint_GetSteering(float us100, float* velLin, float* velAng)
{

}
#endif
