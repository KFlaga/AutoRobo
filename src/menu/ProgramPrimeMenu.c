/*
 * ProgramPrimeMenu.c
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#include "ProgramPrimeMenu.h"
#include "ProgramEditMenu.h"
#include "ProgramExecMenu.h"
#include "ProgramChooseMenu.h"
#include "ButtonModule.h"
#include "MenuTexts.h"
#include "../robo/RoboModule.h"
#include "WaitExecMenu.h"
#include "ErrorMenu.h"

USE_MENU_TEXTS;
Menu _progPrimeMenu;
static uint8_t _curOpt;

void _ppm_update(uint16_t);

void _ppm_fwd_New();
void _ppm_fwd_Edit();
void _ppm_fwd_Exec();
void _ppm_return_Exec(uint16_t status);

static const uint8_t _ppm_opts_count = 3;
typedef void (*MenuOpt)();
 static const MenuOpt _ppm_fwd_opts[] = {
		_ppm_fwd_New,
		_ppm_fwd_Edit,
		_ppm_fwd_Exec
};
static const uint8_t _ppm_opt_txts[] = {
		TXT_Prog_New,
		TXT_Prog_Edit,
		TXT_Prog_Exec
};

void ProgramPrimeMenu_Init(Menu* parent)
{
	_progPrimeMenu.child = 0;
	_progPrimeMenu.parent = parent;
	_progPrimeMenu.onReturn = 0;
	_progPrimeMenu.onUpdate = _ppm_update;
	COPY_MENU_TEXT(TXT_Prog_Main, _progPrimeMenu.lineTop);
	COPY_MENU_TEXT(_ppm_opt_txts[0], _progPrimeMenu.lineBot);

	_curOpt = 0;
}

__inline__ Menu* ProgramPrimeMenu_GetMenu()
{
	return &_progPrimeMenu;
}

void _ppm_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		// Choose current instruction
		_ppm_fwd_opts[_curOpt]();
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		// Return to prime menu
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		// Move to next instruction if available
		if (_curOpt < _ppm_opts_count - 1)
		{
			_curOpt += 1;
			COPY_MENU_TEXT(_ppm_opt_txts[_curOpt], _progPrimeMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		if (_curOpt > 0)
		{
			_curOpt -= 1;
			COPY_MENU_TEXT(_ppm_opt_txts[_curOpt], _progPrimeMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
}

void _ppm_fwd_New()
{
	ProgramNewMenu_Init(&_progPrimeMenu);
	MenuModule_GoToChild(ProgramNewMenu_GetMenu(), 0);
}

void _ppm_fwd_Edit()
{
	ProgramEditMenu_Init(&_progPrimeMenu);
	MenuModule_GoToChild(ProgramEditMenu_GetMenu(), 0);
}

void _ppm_fwd_Exec()
{
	ProgramChooseMenu_Init(&_progPrimeMenu);
	MenuModule_GoToChild(ProgramChooseMenu_GetMenu(), _ppm_return_Exec);
}

void _ppm_return_Exec(uint16_t status)
{
	if(status == PROG_STATUS_CHOOSEN)
	{
		if(RoboModule_ExecuteProgram(ProgramChooseMenu_GetProgram()))
		{
			WaitExecMenu_Init(&_progPrimeMenu);
			MenuModule_GoToChild(WaitExecMenu_GetMenu(),0);
		}
		else
		{
			uchar error[] = { 'N','i','e','p','o','w','o','d','z','e','n','i','e',' ',' ',' ' };
			ErrorMenu_Init(&_progPrimeMenu, error, false);
			MenuModule_GoToChild(ErrorMenu_GetMenu(), 0);
		}
	}
}

