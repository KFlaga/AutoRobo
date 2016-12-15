/*
 * SetParamMenu.c
 *
 *  Created on: Aug 19, 2015
 *      Author: Kamil
 */


#include "SetParamMenu.h"
#include "MenuTexts.h"
#include "ButtonModule.h"


USE_MENU_TEXTS;
Menu _setParamMenu;

static uint8_t _curParam = 0;
RoboInstruction _instruction;
RoboFunDef _funDef;
void* _instPtr;

void _chooseParam(uint8_t param);
void _spm_update(uint16_t pressed);

void SetParamMenu_Init(Menu* parent, RoboInstruction* instruction)
{
	_setParamMenu = (Menu) {
		.parent = parent,
				.child = 0,
				.onUpdate = _spm_update,
				.onReturn = 0
	};

	_curParam = 0;
	COPY_MENU_TEXT(TXT_ChooseParams, _setParamMenu.lineTop);

	_funDef = *(instruction->functionDef);
	_instruction = *instruction;
	_instruction.functionDef = &_funDef;
	_instruction.params[0] = _funDef.paramDefaultVals[0];
	_instruction.params[1] = _funDef.paramDefaultVals[1];
	_instruction.params[2] = _funDef.paramDefaultVals[2];

	_instPtr = (void*)instruction;

	_chooseParam(0);
}

Menu* SetParamMenu_GetMenu()
{
	return &_setParamMenu;
}

void _chooseParam(uint8_t param)
{
	if(param > 0) // Can go to prev param - set arrow
		_setParamMenu.lineTop[14] = CHAR_ARRLEFT;
	else
		_setParamMenu.lineTop[14] = ' ';
	if(param < _funDef.paramCount - 1) // Can go to next param - set arrow
		_setParamMenu.lineTop[15] = CHAR_ARRRIGHT;
	else
		_setParamMenu.lineTop[15] = ' ';

	unsigned char pval[5] = { ' ',' ',' ',' ',' ' };
	uint_to_uchar(_instruction.params[param], pval);

	unsigned char botline[16] = {
			_funDef.paramNames[param], '=',
			' ', pval[0], pval[1] , pval[2], pval[3], pval[4], ' ',
			_funDef.paramUnits[2*param],
			_funDef.paramUnits[2*param+1],
			' ', ' ', ' ', ' ', ' '};

	COPY_MEM(botline,_setParamMenu.lineBot,0,0,16);

	MenuModule_RefreshLcdBot();
	MenuModule_RefreshLcdTop();
}

void _spm_updateParam(uint8_t param)
{
	unsigned char pval[5] = { ' ',' ',' ',' ',' ' };
	uint_to_uchar(_instruction.params[param], pval);

	COPY_MEM(pval,_setParamMenu.lineBot,0,3,5);

	MenuModule_RefreshLcdBot();
}

void _spm_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		// Back to parent with status pwd - params accpeted
		((RoboInstruction*)_instPtr)->params[0] = _instruction.params[0];
		((RoboInstruction*)_instPtr)->params[1] = _instruction.params[1];
		((RoboInstruction*)_instPtr)->params[2] = _instruction.params[2];
		MenuModule_BackToParent(PARAM_STATUS_FWD);
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		// back to parent with staus bwd
		MenuModule_BackToParent(PARAM_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Right)
	{
		// Move to next param if available
		if (_curParam < _funDef.paramCount-1)
		{
			_curParam += 1;
			_chooseParam(_curParam);
		}
		else
		{
			_curParam = 0;
			_chooseParam(_curParam);
		}
	}
	else if (pressed & 1 << BUT_Left)
	{
		// Move to prev param if available
		if (_curParam > 0)
		{
			_curParam -= 1;
			_chooseParam(_curParam);
		}
		else
		{
			_curParam =_funDef.paramCount-1;
			_chooseParam(_curParam);
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		// Increment param
		if (_instruction.params[_curParam] <= 9999 - _funDef.paramIncrement[_curParam])
		{
			_instruction.params[_curParam] += _funDef.paramIncrement[_curParam];
			_spm_updateParam(_curParam);
		}
	}

	else if (pressed & 1 << BUT_Down)
	{
		// Decrement param
		if (_instruction.params[_curParam] >= _funDef.paramIncrement[_curParam])
		{
			_instruction.params[_curParam] -= _funDef.paramIncrement[_curParam];
			_spm_updateParam(_curParam);
		}
	}
}
