#include <movement/RoboProgram.h>
#include <movement/RoboVault.h>
#include "ProgramEditMenu.h"
#include "ProgramSetNameMenu.h"
#include "ProgramPrimeMenu.h"
#include "Memory.h"
#include "MenuTexts.h"
#include "ChooseInstrMenu.h"
#include "ButtonModule.h"

USE_MENU_TEXTS;
Menu _programChangeMenu;
RoboProgram _program;

extern const RoboFunDef _roboFuns[RoboFunction_FunctionsCount];

static int8_t _curParam;
static int8_t _curInstr;
static RoboInstruction _emptyInst = (RoboInstruction) {
	.functionDef = &_roboFuns[0],
			.next = 0,
			.prev = 0,
			.params = { 0,0,0 }
};

static const uchar _returnChars[] = {
		' ',' ',' ','R','e','t','u','r',
		'n',' ',' ',' ',' ',' ',' ',' '
};

static RoboInstruction* _currentInstruction;

void _pnm_chooseName(uint16_t);
void _penm_update(uint16_t pressed);
void _pnm_nameReturn(uint16_t status);
void _penm_newInstrReturn(uint16_t status);
void _penm_editInstReturn(uint16_t status);
void _pem_chooseProgram(uint16_t);
void _pem_chooseProgramReturn(uint16_t status);
void _penm_changeInstructionReturn(uint16_t status);

static bool _isEditMenu;
static uint8_t _progId;

// Program new and change are essentially the same menus
// with exception that on first update of new program name is run
// and new program is created
// and on edit program choose is created
// On return with bwd, both programs are discarded, on fwd
// new program is added to vault and edited is changed
void ProgramNewMenu_Init(Menu* parent)
{
	_programChangeMenu.parent = parent;
	_programChangeMenu.child = 0;
	_programChangeMenu.onUpdate = _pnm_chooseName;
	_programChangeMenu.onReturn = 0;
	_isEditMenu = false;
}

void ProgramEditMenu_Init(Menu* parent)
{
	_programChangeMenu.parent = parent;
	_programChangeMenu.child = 0;
	_programChangeMenu.onUpdate = _pem_chooseProgram;
	_programChangeMenu.onReturn = 0;
	_isEditMenu = true;
}

 Menu* ProgramNewMenu_GetMenu()
{
	return &_programChangeMenu;
}

 Menu* ProgramEditMenu_GetMenu()
{
	return &_programChangeMenu;
}

void _penm_updateInstrNum()
{
	if(_curInstr < 9)
	{
		_programChangeMenu.lineBot[0] = _curInstr + 1 + '0';
		_programChangeMenu.lineBot[1] = '.';
		_programChangeMenu.lineBot[2] = ' ';
	}
	else
	{
		_programChangeMenu.lineBot[0] = (_curInstr+1) / 10 + '0';
		_programChangeMenu.lineBot[1] = (_curInstr+1) % 10 + '0';
		_programChangeMenu.lineBot[2] = '.';
	}
}


void _penm_update(uint16_t pressed)
{
	if (pressed & 1 << BUT_Fwd)
	{
		// if eleceted return - return with saving program
		// if seleceted empty instruction -> go to instruction choose menu
		// else got to instruction edit menu
		if(_curInstr == _program.instCount + 1)
		{
			if(!_isEditMenu)
			{
				RoboProgram* newprog = allocMemory(sizeof(RoboProgram));
				newprog->firstInstruction = _program.firstInstruction;
				newprog->lastInstruction = _program.lastInstruction;
				newprog->instCount = _program.instCount;
				COPY_MEM(_program.name, newprog->name, 0, 0, 12);

				RoboVault_AddProgram(newprog);
			}
			//	else // save changes ( for now save always )
			MenuModule_BackToParent(PROG_STATUS_OK);
		}
		if(_curInstr == _program.instCount)
		{
			if(_curInstr < 99)
			{
				ChooseInstrMenu_Init(&_programChangeMenu);
				MenuModule_GoToChild(ChooseInstrMenu_GetMenu(), _penm_newInstrReturn);
			}
		}
		else
		{

		}
	}
	else if (pressed & 1 << BUT_Bwd)
	{
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
	else if (pressed & 1 << BUT_Down)
	{
		// if selected instruction is return -> do nothing
		// if selected instruction is empty one -> go to return
		// if is last program instruction move to empty one
		// else move down ( if on param view, back to instruction name view )
		if(_curInstr == _program.instCount) // Go from empty to return
		{
			_curInstr++;
			COPY_MEM(_returnChars,
					_programChangeMenu.lineBot, 0, 0, 16);
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
		else if(_curInstr == _program.instCount - 1) // go from last to empty
		{
			_curInstr++;
			COPY_MEM(_emptyInst.functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_penm_updateInstrNum();
			_currentInstruction = 0;
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
		else if(_curInstr < _program.instCount - 1) // go to next instr
		{
			_curInstr++;
			_currentInstruction = _currentInstruction->next;
			COPY_MEM(_currentInstruction->functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_penm_updateInstrNum();
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
	}
	else if (pressed & 1 << BUT_Up)
	{
		// if selected instruction is first one -> do nothing
		// else if empty move to last program instruction
		// else move down
		if(_curInstr == _program.instCount + 1) // go from return to empty
		{
			_curInstr--;
			COPY_MEM(_emptyInst.functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_penm_updateInstrNum();
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
		else if(_curInstr == _program.instCount && _curInstr > 0) // go from empty to last
		{
			_curInstr--;
			_currentInstruction = _program.lastInstruction;
			COPY_MEM(_program.lastInstruction->functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_penm_updateInstrNum();
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
		else if(_curInstr > 0) // go to prev instr
		{
			_curInstr--;
			_currentInstruction = _currentInstruction->next;
			COPY_MEM(_currentInstruction->functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_penm_updateInstrNum();
			_curParam = 0;

			MenuModule_RefreshLcdBot();
		}
	}
	else if (pressed & 1 << BUT_Right)
	{
		// if selected instruction have 0 params or
		// last param is visible, do nothing
		// else move to next param
		if(_curInstr < _program.instCount)
			if(_curParam < _currentInstruction->functionDef->paramCount )
			{
				// move to next param
				_curParam++;
			}
	}
	else if (pressed & 1 << BUT_Left)
	{
		// if view is on param, move to previous one or to inst. name
		if(_curInstr < _program.instCount)
		{
			if(_curParam > 0 )
			{
				// move to prev param
				_curParam--;
			}
		}
	}
}

void _pnm_chooseName(uint16_t p)
{
	// On first run choose new name for program
	ProgramSetNameMenu_Init(&_programChangeMenu);
	MenuModule_GoToChild(ProgramSetNameMenu_GetMenu(),_pnm_nameReturn);
}

void _pnm_nameReturn(uint16_t status)
{
	// Name choosen -> set new update funtion / lcd lines
	if(status == PROG_STATUS_CHOOSEN)
	{
		_programChangeMenu.onUpdate = _penm_update;
		_programChangeMenu.onReturn = 0;
		COPY_MENU_TEXT(TXT_ChooseInstr, _programChangeMenu.lineTop);
		uchar fstEmpty[17] = { '1','.',' ','N','U','L','L',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		_program.instCount = 0;
		COPY_MEM(ProgramSetNameMenu_GetName(), _program.name, 0, 0, 12);
		COPY_MEM(fstEmpty, _programChangeMenu.lineBot, 0, 0, 16);
		_curInstr = 0;
		MenuModule_RefreshLcdBot();
		MenuModule_RefreshLcdTop();
	}
	else
	{
		MenuModule_BackToParent(PROG_STATUS_BWD);
	}
}

void _penm_newInstrReturn(uint16_t status)
{
	// If status != ok = instruction canceled
	// do not append new instruction, just back
	// Else append new instruction and update lcd
	if(status == INST_STATUS_OK)
	{
		_program.instCount++;
		RoboInstruction* newcpy = ChooseInstrMenu_GetInstr();
		RoboInstruction* newInstr = allocMemory(sizeof(RoboInstruction));
		*newInstr = (RoboInstruction) {
			.functionDef = newcpy->functionDef,
					.params = { newcpy->params[0], newcpy->params[1], newcpy->params[2] },
					.next = 0,
					.prev = _program.lastInstruction
		};
		// If
		if( _program.lastInstruction != 0 )
			_program.lastInstruction->next = newInstr;
		else
			_program.firstInstruction = newInstr;
		_program.lastInstruction = newInstr;
		_currentInstruction = newInstr;

		// Update lcd
		COPY_MEM(_currentInstruction->functionDef->name,
				_programChangeMenu.lineBot, 0, 3, 8);
		_penm_updateInstrNum();

		MenuModule_RefreshLcdBot();
	}
}

void _penm_editInstReturn(uint16_t status)
{
	if(status == PROG_STATUS_INST_CHANGED)
	{
		ChooseInstrMenu_Init(&_programChangeMenu);
		MenuModule_GoToChild(ChooseInstrMenu_GetMenu(), _penm_changeInstructionReturn);
	}
	else if(status == PROG_STATUS_INST_REMOVED)
	{
		RoboInstruction* toRemove = _currentInstruction;

		if(_curInstr == _program.instCount - 1) // last
		{
			// set last = previous
			_program.lastInstruction = _program.lastInstruction->prev;
			if(_program.instCount > 1)
				_program.lastInstruction->next = 0;
		}
		else if(_program.instCount > 1)
			_currentInstruction->next->prev = _currentInstruction->prev;
		if(_curInstr == 0) // first
		{
			_program.firstInstruction = _program.firstInstruction->next;
			if(_program.instCount > 1)
				_program.firstInstruction->prev = 0;
		}
		else if(_program.instCount > 1)
			_currentInstruction->prev->next = _currentInstruction->next;
		if(_program.instCount == 1) // no more instructions
		{
			COPY_MEM(_emptyInst.functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
			_currentInstruction = 0;
		}
		else
		{
			if(_curInstr == 0)
				_currentInstruction = _currentInstruction->next;
			else
				_currentInstruction = _currentInstruction->prev;

			COPY_MEM(_currentInstruction->functionDef->name,
					_programChangeMenu.lineBot, 0, 3, 8);
		}

		_program.instCount -= 1;
		freeMemory(toRemove, sizeof(RoboInstruction));
		_curParam = 0;
		MenuModule_RefreshLcdBot();
	}
}


void _penm_changeInstructionReturn(uint16_t status)
{
	if(status == PROG_STATUS_CHOOSEN)
	{
		RoboInstruction* newcpy = ChooseInstrMenu_GetInstr();
		_currentInstruction->functionDef = newcpy->functionDef;
		_currentInstruction->params[0] = newcpy->params[0];
		_currentInstruction->params[2] = newcpy->params[1];
		_currentInstruction->params[1] = newcpy->params[2];

		// Update lcd
		COPY_MEM(_currentInstruction->functionDef->name,
				_programChangeMenu.lineBot, 0, 3, 8);\
		MenuModule_RefreshLcdBot();
	}
}

void _pem_chooseProgram(uint16_t x)
{

}
