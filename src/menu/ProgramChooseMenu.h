/*
 * ProgramPrimeMenu.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMCHOOSEMENU_H_
#define PROGRAMCHOOSEMENU_H_

#include "Core.h"
#include "MenuModule.h"
#include "../robo/RoboProgram.h"

void ProgramChooseMenu_Init(Menu* parent);
__inline__ Menu* ProgramChooseMenu_GetMenu();

__inline__ RoboProgram* ProgramChooseMenu_GetProgram();

#endif /* PROGRAMPRIMEMENU_H_ */
