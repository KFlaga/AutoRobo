#include "ButtonModule.h"
#include "LcdModule.h"

// wait 100ns ~= 11 cycles ( do f.eg. 2 ifs ) between set and check
#define _readButState() CheckPinLow(BUT_MUX_Read_Port, BUT_MUX_Read)

static bool _test = false;
static uint16_t _butStates[3] = { 0,0,0 }; // States : now, last time, 2 times ago
static uint16_t _pressed = 0;
static uint16_t _released = 0;

#define _getButState(but, sec) ((_butStates[sec] & 1<<but)>>but)

#define _isButPressed(but) ( ((_butStates[0] & 1<<but)  && \
		(_butStates[1] & 1<<but) )  && !(_butStates[2] & 1<<but) ) // PPR

#define _isButReleased(but)  ( (!(_butStates[0] & 1<<but)  && \
		!(_butStates[1] & 1<<but) )  && (_butStates[2] & 1<<but) ) // RRP

static void _setMuxPins(uint8_t butCode);

void ButtonModule_Init(Module* module)
{
	module->Start = ButtonModule_Start;
	module->Stop = ButtonModule_Stop;
	module->Update = ButtonModule_Update;

	// Buttons pin in high speed
	InitPinOutput(BUT_MUX_A0_Port, BUT_MUX_A0Num);
	InitPinOutput(BUT_MUX_A1_Port, BUT_MUX_A1Num);
	InitPinOutput(BUT_MUX_EN_Port, BUT_MUX_ENNum);
	InitPinInput(BUT_MUX_Read_Port, BUT_MUX_ReadNum);
	InitPinHighSpeed(BUT_MUX_A0_Port, BUT_MUX_A0Num);
	InitPinHighSpeed(BUT_MUX_A1_Port, BUT_MUX_A1Num);
	InitPinHighSpeed(BUT_MUX_EN_Port, BUT_MUX_ENNum);
	InitPinHighSpeed(BUT_MUX_Read_Port, BUT_MUX_ReadNum);
	InitPinPUPD(BUT_MUX_A0_Port, BUT_MUX_A0Num);
	InitPinPUPD(BUT_MUX_A1_Port, BUT_MUX_A1Num);
	InitPinPUPD(BUT_MUX_EN_Port, BUT_MUX_ENNum);
	InitPinPUPD(BUT_MUX_Read_Port, BUT_MUX_ReadNum);
	InitPinPullUp(BUT_MUX_A0_Port, BUT_MUX_A0Num);
	InitPinPullUp(BUT_MUX_A1_Port, BUT_MUX_A1Num);
	InitPinPullDown(BUT_MUX_EN_Port, BUT_MUX_ENNum);
	InitPinPullUp(BUT_MUX_Read_Port, BUT_MUX_ReadNum);
}

unsigned char linebstates[8];
void ButtonModule_Update(uint16_t interval)
{
	// Set successive buttons pins combinations
	// For each button save its old state
	// Read and update the current state
	uint8_t but;
	_pressed = 0;
	_released = 0;
	// move old states
	_butStates[2] = _butStates[1];
	_butStates[1] = _butStates[0];
	_butStates[0] = 0;
	for(but = 0; but < 8; but++)
	{
		_setMuxPins(but);
		ButState newState = _readButState();
		_butStates[0] |= newState << but;
		// if up and left is down - only up is down ( more specific buts 5 and 4 )
		if(but == 5 && _getButState(4,0) == true )
		{
			_butStates[0] &= ~((uint16_t)1<<but); // reset left but state
		}

		_pressed |= _isButPressed(but) << but;
		_released |= _isButReleased(but) << but;
	}
/*	if(_test)
	{
		if(!LcdIsBusy())
		{
			int i;
			for(i = 0; i < 8; i++)
				if(_pressed & 1<<but)
					linebstates[i] = 'P';
				else if(_released & 1<<but)
					linebstates[i] = 'R';
				else if(_getButState(but,0) == true)
					linebstates[i] = 'l';
				else
					linebstates[i] = 'h';
			LcdModule_WriteLine(2, linebstates, 8);
		}
	}*/
}

void ButtonModule_Start()
{
	// reset button states
	*_butStates = *((uint16_t[3]){ 0, 0, 0 }) ;
	_pressed = 0;
	_released = 0;
}

void ButtonModule_Stop()
{
	*_butStates = *((uint16_t[3]){ 0, 0, 0 });
	_pressed = 0;
	_released = 0;
}

void _setMuxPins(uint8_t code)
{
	if( BUT_A0(code) )
		SetPin(BUT_MUX_A0_Port, BUT_MUX_A0);
	else
		ResetPin(BUT_MUX_A0_Port, BUT_MUX_A0);

	if( BUT_A1(code) )
		SetPin(BUT_MUX_A1_Port, BUT_MUX_A1);
	else
		ResetPin(BUT_MUX_A1_Port, BUT_MUX_A1);

	if( BUT_E(code) )
		SetPin(BUT_MUX_EN_Port, BUT_MUX_EN);
	else
		ResetPin(BUT_MUX_EN_Port, BUT_MUX_EN);

	// Some little delay for GPIO and MUX/Gates to stabilize
	// ( ~= 200ns should do ~= 20 cycles )
	for(code = 0;  code < 4; code++)
		asm("nop");
}

__inline__ uint16_t GetButtonStates()
{
	return _butStates[0];
}

__inline__ uint16_t GetPressedButtons()
{
	return _pressed;
}

__inline__ uint16_t GetReleasedButtons()
{
	return _released;
}

//unsigned char linetest[11] = { 'B','U','T','T','O','N',' ','T','E','S','T' };
void ButtonModule_Test()
{
	_test = true;

	//LcdModule_WriteLine(1, linetest, 11);
}

void ButtonModule_Test1()
{
	_setMuxPins(BUT_Switch1);
}
