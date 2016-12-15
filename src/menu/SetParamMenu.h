/*
 * SetParamMenu.h
 *
 *  Created on: Aug 19, 2015
 *      Author: Kamil
 */

#ifndef SETPARAMMENU_H_
#define SETPARAMMENU_H_

#include "menu/MenuModule.h"
#include "../robo/RoboProgram.h"

#define PARAM_STATUS_FWD 0
#define PARAM_STATUS_BWD 1

void SetParamMenu_Init(Menu* parent, RoboInstruction* instruction);
Menu* SetParamMenu_GetMenu();

#endif /* SETPARAMMENU_H_ */
