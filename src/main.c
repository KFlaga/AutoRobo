#include <movement/LowLevelMotorControl.h>
#include <movement/RoboModule.h>
#include <movement/RoboProgram.h>
#include <movement/RoboVault.h>
#include <Utils.h>
#include "Initialization.h"
#include "ModulesControl.h"
#include "LcdModule.h"
#include "Lcd.h"
#include "Pins.h"
#include "menu/MenuModule.h"
#include "ButtonModule.h"
#include "Encoders.h"

int main(void)
{
	Initialize();

	Module lcdModule;
	LcdModule_Init(&lcdModule);
	Module butModule;
	ButtonModule_Init(&butModule);
	Module encModule;
	Encoders_Init(&encModule);
	Module roboModule;
	RoboModule_Init(&roboModule);
	Module menuModule;
	MenuModule_Init(&menuModule);

	RoboVault_Init();

	InitModuleControl();

	RegisterModule(&lcdModule);
	RegisterModule(&butModule);
	RegisterModule(&encModule);
	RegisterModule(&roboModule);
	RegisterModule(&menuModule);

	StartModuleUpdate();

	Encoders_StartRead();

	bool isRunning = TRUE;
	while (isRunning)
	{

	}

	StopModuleUpdate();
	return 0;
}
