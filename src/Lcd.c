
#include "Lcd.h"
#include "Timers.h"
#include "LcdModule.h"
#include "CallTree.h"

static volatile uint32_t _delayCounter = 0;
#define LCDDelay(ms) _delayCounter = 0; while(_delayCounter < ms)

void LcdInitPins4()
{
	// Set pins as outputs with med speed and pull-up
	InitPinOutput(LCD_D4_Port, LCD_D4_PinNum);
	InitPinOutput(LCD_D5_Port, LCD_D5_PinNum);
	InitPinOutput(LCD_D6_Port, LCD_D6_PinNum);
	InitPinOutput(LCD_D7_Port, LCD_D7_PinNum);
	InitPinOutput(LCD_E_Port, LCD_E_PinNum);
	InitPinOutput(LCD_RS_Port, LCD_RS_PinNum);
	InitPinMedSpeed(LCD_D4_Port, LCD_D4_PinNum);
	InitPinMedSpeed(LCD_D5_Port, LCD_D5_PinNum);
	InitPinMedSpeed(LCD_D6_Port, LCD_D6_PinNum);
	InitPinMedSpeed(LCD_D7_Port, LCD_D7_PinNum);
	InitPinMedSpeed(LCD_E_Port, LCD_E_PinNum);
	InitPinMedSpeed(LCD_RS_Port, LCD_RS_PinNum);
	InitPinPUPD(LCD_D4_Port, LCD_D4_PinNum);
	InitPinPUPD(LCD_D5_Port, LCD_D5_PinNum);
	InitPinPUPD(LCD_D6_Port, LCD_D6_PinNum);
	InitPinPUPD(LCD_D7_Port, LCD_D7_PinNum);
	InitPinPUPD(LCD_E_Port, LCD_E_PinNum);
	InitPinPUPD(LCD_RS_Port, LCD_RS_PinNum);
	InitPinPullUp(LCD_D4_Port, LCD_D4_PinNum);
	InitPinPullUp(LCD_D5_Port, LCD_D5_PinNum);
	InitPinPullUp(LCD_D6_Port, LCD_D6_PinNum);
	InitPinPullUp(LCD_D7_Port, LCD_D7_PinNum);
	InitPinPullUp(LCD_E_Port, LCD_E_PinNum);
	InitPinPullUp(LCD_RS_Port, LCD_RS_PinNum);

	ResetPin(LCD_E_Port, LCD_E_Pin);
}

void LcdInit4Byte(bool wait_powerup)
{
	RegisterTickCounter_NOASSERT(&_delayCounter, SLOTC_LCDDelay);

	if(wait_powerup)
	{
		LCDDelay(100);
	}
	LcdSetRSWriteCommand();
	// Reset lcd and set 4bit interface
	LcdWrite4(3);
	LCDDelay(25);
	LcdWrite4(3);
	LCDDelay(5);
	LcdWrite4(3);
	LCDDelay(5);

	// Enable 4 bit mode
	LcdWrite4(2);
	LCDDelay(5);

	// Set default operating options
	LcdWrite8(0x28); // Set 4bit / 2lines
	LcdWrite8(0x08); // Display off
	LcdWrite8(0x01); // Clear screen
	LcdWrite8(0x06); // Set increment cursor
	LcdWrite8(0x0F); // Set diplay/cursor on

	LcdSetRSWriteData();
}

void LcdWrite4(uint8_t data)
{
	if(data & 1)
		SetPin(LCD_D4_Port, LCD_D4_Pin);
	else
		ResetPin(LCD_D4_Port, LCD_D4_Pin);
	if(data & 2)
		SetPin(LCD_D5_Port, LCD_D5_Pin);
	else
		ResetPin(LCD_D5_Port, LCD_D5_Pin);
	if(data & 4)
		SetPin(LCD_D6_Port, LCD_D6_Pin);
	else
		ResetPin(LCD_D6_Port, LCD_D6_Pin);
	if(data & 8)
		SetPin(LCD_D7_Port, LCD_D7_Pin);
	else
		ResetPin(LCD_D7_Port, LCD_D7_Pin);

	LcdEnable();
	LCDDelay(1);
	LcdDisable();
}

void LcdWrite8(uint8_t data)
{
	LcdWrite4High(data);
	LCDDelay(2);
	LcdWrite4Low(data);
	LCDDelay(5);
}


void LcdClear4()
{
	LcdSetRSWriteCommand();
	LcdWrite4(0);
	LCDDelay(2);
	LcdWrite4(1);
	LCDDelay(5);
	LcdSetRSWriteData();
}

void LcdHome4()
{
	LcdSetRSWriteCommand();
	LcdWrite4(0);
	LCDDelay(2);
	LcdWrite4(2);
	LCDDelay(5);
	LcdSetRSWriteData();
}

/* --------------------------------------------------------
 * No-Delay Zone
 * Below operate with wait and fun_ptrs
 * --------------------------------------------------------
 */

static CallTree* _lcdCallTree;

#define _LCDDescend() CALLTREE_DESCEND(_lcdCallTree)
#define _LCDAscend(fun) CALLTREE_ASCEND(fun, _lcdCallTree)

void _lcdWrite8Begin(uint8_t data);
void _lcdWrite8End();
void _lcdWrite4Begin(uint8_t data);
void _lcdWrite4End();
void _lcdDescendFun();

void LcdInitNoDelay()
{
	CALLTREE_ALLOC(_lcdCallTree, 6);
	_lcdCallTree->curFun = 0;
}

 void LcdWrite_NoDelay(uint8_t data)
{
	LcdSetRSWriteData();
	_lcdWrite8Begin(data);
}

void _lcdWrite4Begin(uint8_t data)
{
	if(data & 1) 	SetPin(LCD_D4_Port, LCD_D4_Pin);
	else 			ResetPin(LCD_D4_Port, LCD_D4_Pin);
	if(data & 2) 	SetPin(LCD_D5_Port, LCD_D5_Pin);
	else 			ResetPin(LCD_D5_Port, LCD_D5_Pin);
	if(data & 4) 	SetPin(LCD_D6_Port, LCD_D6_Pin);
	else			ResetPin(LCD_D6_Port, LCD_D6_Pin);
	if(data & 8)	SetPin(LCD_D7_Port, LCD_D7_Pin);
	else			ResetPin(LCD_D7_Port, LCD_D7_Pin);

	LcdEnable();
	LCDWait(1, _lcdWrite4End);
}

void _lcdWrite4End()
{
	LcdDisable();
	// if called within awaiting tree, descend now
	_LCDDescend();
}

static bool _wrt8_isDone = FALSE;
static uint8_t _wrt8_data;
void _lcdWrite8Begin(uint8_t data)
{
	// Set init write data
	_wrt8_isDone = FALSE;
	_wrt8_data = data;
	// Set next call after writing4 is done
	_LCDAscend(_lcdWrite8End);

	_lcdWrite4Begin(data>>4);
}

void _lcdWrite8End()
{
	if(_wrt8_isDone == FALSE) // Written 4bits so far
	{
		_wrt8_isDone = TRUE;
		_LCDAscend(_lcdWrite8End);
		_lcdWrite4Begin(_wrt8_data);
	}
	else // Done writing -> give some time for lcd
	{
		LCDWait(2, _lcdDescendFun);
	}
}

void _lcdDescendFun()
{
	// if called within awaiting tree, descend now
	_LCDDescend();
}

// ----------------------------------------- //
// Write line/lines section
// ----------------------------------------- //

static uint8_t _dataToWrite[32];
static uint8_t _remainigData = 0;

void _lcdWriteMulti();

void LcdWriteLine_NoDelay(uint8_t linenum, uint8_t* data, uint8_t size)
{
	// Save data to write for later
	_remainigData = size;
	*((uint8_t*)_dataToWrite) = *data;
	int i;
	for(i = 0; i < size; i++)
	{
		_dataToWrite[i] = data[size-i-1];
	}
	// Set position to be begin of line 1
	_LCDAscend(_lcdWriteMulti);
	LcdSetRSWriteCommand();
	if(linenum == 2)
		_lcdWrite8Begin(0xC0); // 0b1 1000000
	else
		_lcdWrite8Begin(0x80); // 0b1 0000000
}

void _lcdWriteMulti()
{
	if(_remainigData > 0 )
	{
		LcdSetRSWriteData();
		_LCDAscend(_lcdWriteMulti);
		_remainigData--;
		_lcdWrite8Begin(_dataToWrite[_remainigData]);
	}
	else
	{
		LcdModule_WriteEnd();
		_LCDDescend();
	}
}
