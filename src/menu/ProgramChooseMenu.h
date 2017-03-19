/*
 * ProgramPrimeMenu.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMCHOOSEMENU_H_
#define PROGRAMCHOOSEMENU_H_

#include <movement/RoboProgram.h>
#include <Utils.h>
#include "MenuModule.h"

void ProgramChooseMenu_Init(Menu* parent);
 Menu* ProgramChooseMenu_GetMenu();

 RoboProgram* ProgramChooseMenu_GetProgram();

#endif /* PROGRAMPRIMEMENU_H_ */
