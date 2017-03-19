#include "MenuModule.h"
#include "Memory.h"
#include "PrimeMenu.h"
#include "LcdModule.h"
#include "ButtonModule.h"

static Menu* _currentMenu;
static uint8_t _pendingFlags;
static uint16_t _childStatus;

#define pending_RefreshTop 1
#define pending_RefreshBot 2
#define pending_GoToChild 4
#define pending_BackToParent 8


void MenuModule_Init(Module* module)
{
	module->Start = MenuModule_Start;
	module->Stop = MenuModule_Stop;
	module->Update = MenuModule_Update;

	_currentMenu = 0;
	_pendingFlags = 0;
}

void MenuModule_Update(uint16_t interval)
{
	if(_currentMenu != 0)
	{
		if(_pendingFlags & pending_BackToParent)
		{
			_currentMenu = _currentMenu->parent;
			_pendingFlags = pending_RefreshBot | pending_RefreshTop;
			if(_currentMenu->onReturn != 0)
				_currentMenu->onReturn(_childStatus);
		}
		else if(_pendingFlags & pending_GoToChild)
		{
			_currentMenu = _currentMenu->child;
			_pendingFlags = pending_RefreshBot | pending_RefreshTop;
		}
		else if(_pendingFlags & pending_RefreshTop) // Top have priority over Bot
		{
			if(LcdModule_WriteLine(1, _currentMenu->lineTop, 16))
			{
				_pendingFlags &= ~((uint8_t)pending_RefreshTop);
			}
		}
		else if(_pendingFlags & pending_RefreshBot)
		{
			if(LcdModule_WriteLine(2, _currentMenu->lineBot, 16))
			{
				_pendingFlags &= ~((uint8_t)pending_RefreshBot);
			}
		}
		_currentMenu->onUpdate(GetPressedButtons());
	}
}

void MenuModule_Start()
{
	PrimeMenu_Init();
	_currentMenu = PrimeMenu_GetMenu();
	_pendingFlags = pending_RefreshBot | pending_RefreshTop;
}

void MenuModule_Stop()
{
	_currentMenu = 0;
	_pendingFlags = 0;
}

 void MenuModule_RefreshLcdTop()
{
	_pendingFlags |= pending_RefreshTop;
}

 void MenuModule_RefreshLcdBot()
{
	_pendingFlags |= pending_RefreshBot;
}

 void MenuModule_GoToChild(Menu* child, MenuHandler onReturn)
{
	if(child != 0)
	{
		_pendingFlags |= pending_GoToChild;
		_currentMenu->child = child;
		_currentMenu->onReturn = onReturn;
	}
}

 void MenuModule_BackToParent(uint16_t status)
{
	if(_currentMenu != 0 && _currentMenu->parent != 0)
	{
		_childStatus = status;
		_pendingFlags |= pending_BackToParent;
	}
}
