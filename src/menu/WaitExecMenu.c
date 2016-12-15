

#include "WaitExecMenu.h"
#include "MenuTexts.h"
#include "../robo/RoboModule.h"

USE_MENU_TEXTS;
Menu _waitExecMenu;
static uint16_t _arrCounter;
static uint8_t _arrPos;
static int8_t _arrIncrem;
static uint8_t _arrChar;

void _wem_update(uint16_t buts);

void WaitExecMenu_Init(Menu* parent)
{
	_waitExecMenu.child = 0;
	_waitExecMenu.parent = parent;
	COPY_MENU_TEXT(TXT_WaitForExec_Top, _waitExecMenu.lineTop);
	COPY_MENU_TEXT(TXT_WaitForExec_Bot, _waitExecMenu.lineBot);
	_waitExecMenu.onUpdate = _wem_update;
	_waitExecMenu.onReturn = 0;
	_arrCounter = 0;
	_arrPos = 0;
	_arrIncrem = 1;
	_arrChar = CHAR_ARRRIGHT;
}

__inline__ Menu* WaitExecMenu_GetMenu()
{
	return &_waitExecMenu;
}

void _wem_update(uint16_t pressed)
{
	if(!RoboModule_IsBusy())
	{
		MenuModule_BackToParent(0);
	}
	// Animate arrow
	if(_arrCounter < 80)
		_arrCounter++;
	else
	{
		_waitExecMenu.lineBot[8+_arrPos] = ' ';
		_arrCounter = 0;
		if(_arrPos == 7)
		{
			_arrIncrem = -1;
			_arrChar = CHAR_ARRLEFT;
		}
		else if(_arrPos == 0 )
		{
			_arrIncrem = 1;
			_arrChar = CHAR_ARRRIGHT;
		}
		_arrPos += _arrIncrem;
		_waitExecMenu.lineBot[8+_arrPos] = _arrChar;

		MenuModule_RefreshLcdBot();
	}
}
