#include "ProgramChooseMenu.h"
#include "ProgramPrimeMenu.h"
#include "ButtonModule.h"
#include "MenuTexts.h"
#include "../robo/RoboVault.h"


USE_MENU_TEXTS;
Menu _progChooseMenu;
static uint8_t _curProg;

void _pcm_update(uint16_t);

void ProgramChooseMenu_Init(Menu* parent)
{
	_progChooseMenu.child = 0;
	_progChooseMenu.parent = parent;
	_progChooseMenu.onReturn = 0;
	_progChooseMenu.onUpdate = _pcm_update;
	COPY_MENU_TEXT(TXT_Prog_Choose, _progChooseMenu.lineTop);
	COPY_MEM("NO_PROGRAMS      ",_progChooseMenu.lineBot,0,0,16);

	_curProg = 0;
	if(RoboVault_GetProgramCount() > 0)
		COPY_MEM(RoboVault_GetProgramById(_curProg)->name,_progChooseMenu.lineBot,0,0,12);
}

__inline__ Menu* ProgramChooseMenu_GetMenu()
{
	return &_progChooseMenu;
}

__inline__ RoboProgram* ProgramChooseMenu_GetProgram()
{
	return RoboVault_GetProgramById(_curProg);
}

void _pcm_update(uint16_t pressed)
{
	if ((pressed & 1 << BUT_Fwd) && (RoboVault_GetProgramCount() > 0))
	{
		// Return to parent with program choosen
		MenuModule_BackToParent(PROG_STATUS_CHOOSEN);
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		if(_curProg < RoboVault_GetProgramCount() - 1)
		{
			_curProg += 1;
			COPY_MEM(RoboVault_GetProgramById(_curProg)->name,_progChooseMenu.lineBot,0,0,12);
			MenuModule_RefreshLcdBot();
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		if(_curProg > 0)
		{
			_curProg -= 1;
			COPY_MEM(RoboVault_GetProgramById(_curProg)->name,_progChooseMenu.lineBot,0,0,12);
			MenuModule_RefreshLcdBot();
		}
	}
}
