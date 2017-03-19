/*
 * Interface for robo module for external use
 */

#ifndef ROBOMODULE_H_
#define ROBOMODULE_H_

#include <movement/RoboMovementSensor.h>
#include <movement/RoboProgram.h>
#include <Utils.h>
#include "ModulesControl.h"
#include "CallTree.h"

typedef enum _RoboState
{
	RoboState_Idle = 0x0, // Nothing's going on (no program executed, module running)
	RoboState_Busy = 0x1, // Program is executed
	RoboState_Stopped = 0x2, // Module is stopped
	RoboState_Waiting = RoboState_Busy | 0x4,
	RoboState_Moving = RoboState_Busy | 0x8,
	RoboState_InstuctionEnd = RoboState_Busy | 0x10,
} RoboState;

typedef enum _RoboError
{
	RoboError_NoError = 0,
	RoboError_Busy = 1,
	RoboError_Exe_EmptyProg = 3,
	RoboError_Exe_NullProg = 4,
	RoboError_Exe_NullInst = 5,
	RoboError_Exe_Unknown = 6,
} RoboError;

void RoboModule_Init(Module* module);
bool RoboModule_IsBusy();
void RoboModule_Start();
void RoboModule_Stop();
void RoboModule_Update(uint16_t interval);
bool RoboModule_ExecuteProgram(RoboProgram* program);
bool RoboModule_ExecuteInstrucion(RoboInstruction* instrucion);
RoboState RoboModule_GetState();
RoboError RoboModule_GetError();

// GetMotorStates

#endif /* ROBOMODULE_H_ */
