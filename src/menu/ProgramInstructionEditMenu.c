#include "ProgramInstructionEditMenu.h"
#include "ButtonModule.h"
#include "ProgramPrimeMenu.h"
#include "MenuTexts.h"

USE_MENU_TEXTS;
static Menu _progInstrEditMenu;

void _piem_update(uint16_t pressed);

static const uint8_t _piem_opts_count = 2;
static const uint8_t _piem_opts_txt[2] = {
		TXT_Prog_InstrChange,
		TXT_Prog_InstrRemove
};
static const uint8_t _piem_opts_status[2] = {
		PROG_STATUS_INST_CHANGED,
		PROG_STATUS_INST_REMOVED
};
static uint8_t _curOpt;

void ProgramInstructionEditMenu_Init(Menu* parent)
{
	_progInstrEditMenu.child = 0;
	_progInstrEditMenu.parent = parent;
	_progInstrEditMenu.onReturn = 0;
	_progInstrEditMenu.onUpdate = _piem_update;
	COPY_MENU_TEXT(TXT_Prog_InstrEdit, _progInstrEditMenu.lineTop);
	COPY_MENU_TEXT(TXT_Prog_InstrChange,_progInstrEditMenu.lineBot);
	_curOpt = 0;
}

 Menu* ProgramInstructionEditMenu_GetMenu()
{
	return &_progInstrEditMenu;
}

void _piem_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		MenuModule_BackToParent(_piem_opts_status[_curOpt]);
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		if(_curOpt < _piem_opts_count - 1)
		{
			_curOpt++;
			COPY_MENU_TEXT(_piem_opts_txt[_curOpt],_progInstrEditMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		if(_curOpt > 0)
		{
			_curOpt--;
			COPY_MENU_TEXT(_piem_opts_txt[_curOpt],_progInstrEditMenu.lineBot);

			MenuModule_RefreshLcdBot();
		}
	}
}
