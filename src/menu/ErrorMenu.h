/*
 * ErrorMenu.h
 *
 *  Created on: Aug 19, 2015
 *      Author: Kamil
 */

#ifndef ERRORMENU_H_
#define ERRORMENU_H_

#include "Core.h"
#include "MenuModule.h"

// Msg must have size of 16
// If error is critical must reset, if not can go bwd
void ErrorMenu_Init(Menu* parent, unsigned char* msg, bool is_critical);
__inline__ Menu* ErrorMenu_GetMenu();

#endif /* ERRORMENU_H_ */
