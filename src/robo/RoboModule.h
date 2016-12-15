/*
 * Interface for robo module for external use
 */

#ifndef ROBOMODULE_H_
#define ROBOMODULE_H_

#include "Core.h"
#include "ModulesControl.h"
#include "CallTree.h"
#include "RoboProgram.h"

typedef enum _RoboState
{
	Robo_Busy = 1,
	Robo_Idle = 0,
	Robo_Stopped = 2
} RoboState;
typedef enum _RoboError
{
	Robo_Err_NoError = 0,
	Robo_Err_Busy = 1,
	Robo_ExeErr_EmptyProg = 3,
	Robo_ExeErr_NullProg = 4,
	Robo_ExeErr_NullInst = 5,
	Robo_ExeErr_Unknown = 6,

} RoboError;

void RoboModule_Init(Module* module);
__inline__ bool RoboModule_IsBusy();
void RoboModule_Start();
void RoboModule_Stop();
void RoboModule_Update(uint16_t interval);
bool RoboModule_ExecuteProgram(RoboProgram* program);
bool RoboModule_ExecuteInstrucion(RoboInstruction* instrucion);
__inline__ RoboState RoboModule_GetState();
__inline__ RoboError RoboModule_GetError();

// GetMotorStates

#endif /* ROBOMODULE_H_ */
