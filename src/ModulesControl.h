#ifndef MODULESCONTROL_H_
#define MODULESCONTROL_H_

#include "Core.h"

#define MODULE_TICK 50 // x 100us

typedef void(*ModuleUpdateFunction)(uint16_t);
typedef void(*ModuleFunction)();


typedef struct _module
{
	ModuleUpdateFunction Update;
	ModuleFunction Start;
	ModuleFunction Stop;
	uint16_t lastUpdateCNT;
} Module;

// Init modules timer
void InitModuleControl();

void StartModuleUpdate();
void StopModuleUpdate();

bool RegisterModule(Module* module);
bool UnregisterModule(Module* module);

#endif
