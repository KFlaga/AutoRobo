#include <movement/RoboModule.h>
#include "PrimeMenu.h"
#include "MenuTexts.h"
#include "ButtonModule.h"
#include "ChooseInstrMenu.h"
#include "WaitExecMenu.h"
#include "ProgramPrimeMenu.h"

USE_MENU_TEXTS;
Menu _primeMenu;

void _pm_update(uint16_t buts);

typedef void(*MenuOpt)();
void _pm_fwd_ExeInst();
void _pm_fwd_Reset();
void _pm_back_ExeInst(uint16_t status);
void _pm_fwd_Programs();

#define _pm_opts_count 3
static uint8_t _curOpt = 0;
static const MenuOpt _pm_fwd_opts[_pm_opts_count] = {
		_pm_fwd_ExeInst,
		_pm_fwd_Programs,
		_pm_fwd_Reset
};
static const uint8_t _pm_text_opts[_pm_opts_count] = {
		TXT_Prime_ExeInst,
		TXT_Prime_Program,
		TXT_Prime_Reset
};

void PrimeMenu_Init()
{
	_primeMenu.child = 0;
	_primeMenu.parent = 0;
	COPY_MENU_TEXT(TXT_Prime_Top, _primeMenu.lineTop);
	COPY_MENU_TEXT(TXT_Prime_ExeInst, _primeMenu.lineBot);
	_curOpt = 0;
	_primeMenu.onUpdate = _pm_update;
	_primeMenu.onReturn = 0;
}

 Menu* PrimeMenu_GetMenu()
{
	return &_primeMenu;
}

void _pm_update(uint16_t pressed)
{
	if(pressed & 1<<BUT_Fwd)
	{
		// Choose current instruction
		_pm_fwd_opts[_curOpt]();
	}
	else if(pressed & 1<<BUT_Down)
	{
		// Move to next instruction if available
		if(_curOpt < _pm_opts_count - 1)
		{
			_curOpt += 1;
			COPY_MENU_TEXT(_pm_text_opts[_curOpt], _primeMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
	else if(pressed & 1<<BUT_Up)
	{
		if(_curOpt > 0)
		{
			_curOpt -= 1;
			COPY_MENU_TEXT(_pm_text_opts[_curOpt], _primeMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
}

void _pm_fwd_ExeInst()
{
	ChooseInstrMenu_Init(&_primeMenu);
	MenuModule_GoToChild(ChooseInstrMenu_GetMenu(), _pm_back_ExeInst);
}

void _pm_back_ExeInst(uint16_t status)
{
	if(status == INST_STATUS_OK)
	{
		// Execute instruction
		RoboModule_ExecuteInstrucion(ChooseInstrMenu_GetInstr());
		WaitExecMenu_Init(&_primeMenu);
		MenuModule_GoToChild(WaitExecMenu_GetMenu(),0);
	}
	else
	{
		// Do nothing probably
	}
}

void _pm_fwd_Reset()
{
	NVIC_SystemReset();
}


void _pm_fwd_Programs()
{
	ProgramPrimeMenu_Init(&_primeMenu);
	MenuModule_GoToChild(ProgramPrimeMenu_GetMenu(), 0);
}
