#include "LcdModule.h"
#include "Lcd.h"

 static bool _lcdTurnedOn = FALSE;
 static bool volatile _lcdBusy = FALSE;

 static bool volatile _isWaiting = FALSE;
 static uint16_t volatile _waitx100us = 0;
 static LCDFunction volatile _onWaitEnd = 0;

void LcdModule_Init(Module* module)
{
	module->Start = LcdModule_Start;
	module->Stop = LcdModule_Stop;
	module->Update = LcdModule_Update;

	LcdInitPins4();
	LcdInit4Byte(TRUE);
	LcdHome4();
	LcdInitNoDelay();

	_lcdTurnedOn = TRUE;
	_lcdBusy = FALSE;
	_isWaiting = FALSE;
}

void LcdModule_Start()
{
	_lcdBusy = FALSE;
	_isWaiting = FALSE;
}

void LcdModule_Stop()
{
	_lcdBusy = true;
}

void LCDWait(uint16_t ms, LCDFunction onContinue)
{
	_waitx100us = ms*10;
	_onWaitEnd = onContinue;
	_isWaiting = true;
}

 void LcdModule_WriteEnd()
{
	_lcdBusy = false;
}

void LcdModule_Update(uint16_t interval)
{
	// interval is in x100us
	if(_isWaiting)
	{
		if(interval >= _waitx100us)
		{
			_isWaiting = FALSE;
			_waitx100us = 0;
			if(_onWaitEnd != 0)
				_onWaitEnd();
		}
		else
			_waitx100us -= interval;
	}
}

 void LcdTurnedOn()
{
	LcdInit4Byte(TRUE);
	_lcdTurnedOn = TRUE;
}

 void LcdTurnedOff()
{
	_lcdTurnedOn = FALSE;
	_isWaiting = FALSE;
	_onWaitEnd = 0;
	_waitx100us = 0;
}

 bool LcdIsOn()
{
	return _lcdTurnedOn;
}

 bool LcdIsBusy()
{
	return _lcdBusy;
}

 bool LcdModule_WriteLine(uint8_t line, uint8_t* data, uint8_t size)
{
	if(!_isWaiting)
	{
		_lcdBusy = TRUE;
		LcdWriteLine_NoDelay(line, data, size);
		return true;
	}
	return false;
}

