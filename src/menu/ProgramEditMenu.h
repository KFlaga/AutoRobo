/*
 * ProgramBuildMenu.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMBUILDMENU_H_
#define PROGRAMBUILDMENU_H_

#include "Core.h"
#include "MenuModule.h"

void ProgramNewMenu_Init(Menu* parent);
__inline__ Menu* ProgramNewMenu_GetMenu();

void ProgramEditMenu_Init(Menu* parent);
__inline__ Menu* ProgramEditMenu_GetMenu();

#endif /* PROGRAMBUILDMENU_H_ */
