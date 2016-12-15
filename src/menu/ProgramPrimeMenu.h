/*
 * ProgramPrimeMenu.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMPRIMEMENU_H_
#define PROGRAMPRIMEMENU_H_

#include "Core.h"
#include "MenuModule.h"

void ProgramPrimeMenu_Init(Menu* parent);
__inline__ Menu* ProgramPrimeMenu_GetMenu();

#define PROG_STATUS_OK 0
#define PROG_STATUS_BWD 1
#define PROG_STATUS_CHOOSEN 2

#define PROG_STATUS_INST_REMOVED 3
#define PROG_STATUS_INST_ADDAFTER 4
#define PROG_STATUS_INST_ADDBEFORE 5
#define PROG_STATUS_INST_CHANGED 6

#endif /* PROGRAMPRIMEMENU_H_ */
