/*
 * RoboMovementAlgorithm.h
 *
 *  Created on: Mar 5, 2017
 *      Author: Kamil
 */

#ifndef MOVEMENT_ROBOMOVEMENTALGORITHM_H_
#define MOVEMENT_ROBOMOVEMENTALGORITHM_H_

#include <movement/RoboMovementSensor.h>
#include <movement/RoboProgram.h>

typedef void(*SteeringFunc)(float, float* , float* );
typedef void(*InstructionFunc)(RoboInstruction*);
typedef bool(*CheckFunc)();

typedef struct _MovementAlgorithm
{
	// Sets next movement instruction
	InstructionFunc SetCurrentInstruction;

	// Computes velocities for current frame to reach current target.
	// Arguments:
	// 1) [in]  dT : current frame time in 100us
	// 2) [out] Vt : pointer to computed linear velocity
	// 2) [out] Wt : pointer to computed angular velocity
	SteeringFunc GetSteeringForFrame;

	// Returns true if robot reached current target
	CheckFunc CheckIsTargetReached;
} MovementAlgorithm;

enum MovementAlgorithmType
{
	// Tries to reach target in time-wise optimal way
	// Doesn't follow any strict path
	MovementAlgorithmType_FastestToPoint,
	// Just rotates to target point and moves
	MovementAlgorithmType_RotateAndGo,
};

void SetMovementAlgorithmType(enum MovementAlgorithmType type);
MovementAlgorithm* GetMovementAlgorithm();

#endif /* MOVEMENT_ROBOMOVEMENTALGORITHM_H_ */
