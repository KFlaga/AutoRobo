/*
 * MenuExecProgram.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef MENUEXECPROGRAM_H_
#define MENUEXECPROGRAM_H_

#include <Utils.h>
#include "MenuModule.h"

void ProgramExecMenu_Init(Menu* parent);
 Menu* ProgramExecMenu_GetMenu();

// On first update -> go to choose program menu as child
// On return if(bwd) -> return to parent (progprime)
// if(ok) -> program exec viewer (this one)

#endif /* MENUEXECPROGRAM_H_ */
