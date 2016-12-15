/*
 * ProgramSetNameMenu.h
 *
 *  Created on: Aug 21, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMSETNAMEMENU_H_
#define PROGRAMSETNAMEMENU_H_

#include "Core.h"
#include "MenuModule.h"

void ProgramSetNameMenu_Init(Menu* parent);
__inline__ Menu* ProgramSetNameMenu_GetMenu();

__inline__ uchar* ProgramSetNameMenu_GetName();

#endif /* PROGRAMSETNAMEMENU_H_ */
