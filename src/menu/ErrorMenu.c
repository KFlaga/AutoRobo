/*
 * ErrorMenu.c
 *
 *  Created on: Aug 19, 2015
 *      Author: Kamil
 */


#include "ErrorMenu.h"
#include "MenuTexts.h"
#include "ButtonModule.h"

USE_MENU_TEXTS;
Menu _errorMenu;
static bool _isCritical;

void _erm_update(uint16_t);

void ErrorMenu_Init(Menu* parent, unsigned char* msg, bool is_critical)
{
	_errorMenu.parent = parent;
	_errorMenu.child = 0;
	_isCritical = is_critical;
	_errorMenu.onReturn = 0;
	_errorMenu.onUpdate = _erm_update;

	COPY_MENU_TEXT(TXT_Error_Top, _errorMenu.lineTop);
	COPY_MEM(msg, _errorMenu.lineBot, 0, 0, 16);
}

 Menu* ErrorMenu_GetMenu()
{
	return &_errorMenu;
}

void _erm_update(uint16_t pressed)
{
	if(!_isCritical && (pressed & 1<<BUT_Bwd))
	{
		MenuModule_BackToParent(0);
	}
}
