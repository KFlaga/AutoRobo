/*
 * ProgramSetNameMenu.c
 *
 *  Created on: Aug 21, 2015
 *      Author: Kamil
 */

#include "ProgramSetNameMenu.h"
#include "MenuTexts.h"
#include "ButtonModule.h"
#include "ProgramPrimeMenu.h"
#include "../robo/RoboVault.h"

USE_MENU_TEXTS;
Menu _programNameMenu;
uint8_t _newNameFinal[16] = {
		'n','o','_','_','_','_','_','_',
		'_','_','_','_',' ',' ',' ',' '
};
static uint8_t _newNameCode[12] = {
		13, 14, 34, 13, 0, 12,
		4, 34, 34, 34, 34, 34
};

static const uint8_t _charSetCount = 36;
static uchar _charSet[36] = {
		'a','b','c','d','e','f','g','h','i','j',
		'k','l','m','n','o','p','r','s','t','u',
		'v','w','x','y','z','0','1','2','3','4',
		'5','6','7','8','9','_'
};

static uint8_t _curChar;

void _psnm_update(uint16_t pressed);

void ProgramSetNameMenu_Init(Menu* parent)
{
	_programNameMenu = (Menu) {
		.parent = parent,
		.child  = 0,
		.onReturn = 0,
		.onUpdate = _psnm_update,
		.lineBot = { 'n','o','_','_','_','_','_','_',
					 '_','_','_','_',' ',' ',' ',' ' }
	};
	COPY_MENU_TEXT(TXT_Prog_NewName,_programNameMenu.lineTop);
}

__inline__ Menu* ProgramSetNameMenu_GetMenu()
{
	return &_programNameMenu;
}

__inline__ uchar* ProgramSetNameMenu_GetName()
{
	return _newNameFinal;
}

void _psnm_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		if(RoboVault_GetProgramByName(_newNameFinal) != 0)
		{
			// Program with this name exist
			COPY_MENU_TEXT(TXT_Prog_NameExists,_programNameMenu.lineTop);
		}
		else
		{
			// Return to parent with name choosen
			MenuModule_BackToParent(PROG_STATUS_CHOOSEN);
		}
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		if(_newNameCode[_curChar] < _charSetCount - 1)
			_newNameCode[_curChar] += 1;
		else
			_newNameCode[_curChar] = 0;

		_newNameFinal[_curChar] = _charSet[_newNameCode[_curChar]];
		_programNameMenu.lineBot[_curChar] = _newNameFinal[_curChar];
		MenuModule_RefreshLcdBot();
	}
	else if (pressed & 1 << BUT_Up)
	{
		if(_newNameCode[_curChar] > 0)
			_newNameCode[_curChar] -= 1;
		else
			_newNameCode[_curChar] = _charSetCount - 1;

		_newNameFinal[_curChar] = _charSet[_newNameCode[_curChar]];
		_programNameMenu.lineBot[_curChar] = _newNameFinal[_curChar];
		MenuModule_RefreshLcdBot();
	}
	else if (pressed & 1 << BUT_Right)
	{
		if(_curChar < 10)
			_curChar += 1;
		else
			_curChar = 0;
	}
	else if (pressed & 1 << BUT_Left)
	{
		if(_curChar > 0)
			_curChar -= 1;
		else
			_curChar = 11;
	}
	// MenuModule_SetCursorPos(2, _curChar); // line / pos
}
