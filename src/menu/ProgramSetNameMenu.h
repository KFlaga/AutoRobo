/*
 * ProgramSetNameMenu.h
 *
 *  Created on: Aug 21, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMSETNAMEMENU_H_
#define PROGRAMSETNAMEMENU_H_

#include <Utils.h>
#include "MenuModule.h"

void ProgramSetNameMenu_Init(Menu* parent);
 Menu* ProgramSetNameMenu_GetMenu();

 uchar* ProgramSetNameMenu_GetName();

#endif /* PROGRAMSETNAMEMENU_H_ */
