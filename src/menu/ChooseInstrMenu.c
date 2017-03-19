#include "ChooseInstrMenu.h"
#include "MenuModule.h"
#include "MenuTexts.h"
#include "ButtonModule.h"
#include "SetParamMenu.h"

USE_MENU_TEXTS;
Menu _chooseInstrMenu;
RoboInstruction _instruction;

void _cim_update(uint16_t buts);
void _cim_acceptParams(uint16_t paramStatus);

#define _cim_instr_count (RoboFunction_FunctionsCount - 1)

static uint8_t _curInstr; // Counted from 1

void ChangeInst()
{
	_instruction.functionDef = GetRoboFunctionDefinition(_curInstr);
	int size = 8;
	COPY_TEXT(_instruction.functionDef->name, _chooseInstrMenu.lineBot, size, 0);
	uint8_t spaces[8] =
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	size = 8;
	COPY_TEXT(spaces, _chooseInstrMenu.lineBot, size, 8);
	if (_curInstr > 1)
		_chooseInstrMenu.lineTop[15] = '^';
	else
		_chooseInstrMenu.lineTop[15] = ' ';
	if (_curInstr < _cim_instr_count)
		_chooseInstrMenu.lineBot[15] = 'v';
	else
		_chooseInstrMenu.lineBot[15] = ' ';
}

void ChooseInstrMenu_Init(Menu* parent)
{
	_chooseInstrMenu.child = 0;
	_chooseInstrMenu.parent = parent;
	COPY_MENU_TEXT(TXT_ChooseInstr, _chooseInstrMenu.lineTop);
	_curInstr = 1;
	_chooseInstrMenu.onUpdate = _cim_update;
	_chooseInstrMenu.onReturn = 0;

	_instruction = (RoboInstruction) {
		0, { 0, 0, 0 }, 0, 0
	};
	ChangeInst();
}

 Menu* ChooseInstrMenu_GetMenu()
{
	return &_chooseInstrMenu;
}

 RoboInstruction* ChooseInstrMenu_GetInstr()
{
	return &_instruction;
}

void _cim_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		// Choose current instruction
		// Move to choose params
		SetParamMenu_Init(&_chooseInstrMenu, &_instruction);
		MenuModule_GoToChild(SetParamMenu_GetMenu(), _cim_acceptParams);
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		// back to parent with staus bwd
		MenuModule_BackToParent(INST_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		// Move to next instruction if available
		if (_curInstr < _cim_instr_count)
		{
			_curInstr += 1;
			ChangeInst();

			MenuModule_RefreshLcdBot();
			MenuModule_RefreshLcdTop();
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		if (_curInstr > 1)
		{
			_curInstr -= 1;
			ChangeInst();

			MenuModule_RefreshLcdBot();
			MenuModule_RefreshLcdTop();
		}
	}
}

void _cim_acceptParams(uint16_t status)
{
	if (status == PARAM_STATUS_FWD) // Params accepted - execute
	{
		MenuModule_BackToParent(INST_STATUS_OK);
	}
	else // Choice declined - back to instruction select
	{
		// do nothing
	}
}
