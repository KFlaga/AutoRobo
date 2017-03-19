/*
 * Encoders.c
 *
 *  Created on: Aug 24, 2015
 *      Author: Kamil
 */


#include "Encoders.h"
#include "stm32f4xx_exti.h"


static uint16_t _readings[4];
static bool _isEncReading;


void Encoders_Init(Module* module)
{
	module->Start = Encoders_Start;
	module->Stop = Encoders_Stop;
	module->Update = Encoders_Update;

	// Init pins
	InitPinOutput(ENC_Enable_Port, ENC_Enable_PinNum);
	InitPinPUPD(ENC_Enable_Port, ENC_Enable_PinNum);
	InitPinMedSpeed(ENC_Enable_Port, ENC_Enable_PinNum);
	InitPinPullUp(ENC_Enable_Port, ENC_Enable_PinNum);

	InitPinInput(ENC_Port_FL, ENC_PinNum_FL);
	InitPinMedSpeed(ENC_Port_FL, ENC_PinNum_FL);
	InitPinPUPD(ENC_Port_FL, ENC_PinNum_FL);
	InitPinPullUp(ENC_Port_FL, ENC_PinNum_FL);
	InitPinInput(ENC_Port_FR, ENC_PinNum_FR);
	InitPinMedSpeed(ENC_Port_FR, ENC_PinNum_FR);
	InitPinPUPD(ENC_Port_FR, ENC_PinNum_FR);
	InitPinPullUp(ENC_Port_FR, ENC_PinNum_FR);
	InitPinInput(ENC_Port_BL, ENC_PinNum_BL);
	InitPinMedSpeed(ENC_Port_BL, ENC_PinNum_BL);
	InitPinPUPD(ENC_Port_BL, ENC_PinNum_BL);
	InitPinPullUp(ENC_Port_BL, ENC_PinNum_BL);
	InitPinInput(ENC_Port_BR, ENC_PinNum_BR);
	InitPinMedSpeed(ENC_Port_BR, ENC_PinNum_BR);
	InitPinPUPD(ENC_Port_BR, ENC_PinNum_BR);
	InitPinPullUp(ENC_Port_BR, ENC_PinNum_BR);

	DisableEncoders();
}

void Encoders_Start()
{
	Encoders_ResetAll();
}

void Encoders_Stop()
{
	Encoders_StartRead();
}

 uint16_t Encoders_GetReadings(uint8_t enc_num)
{
	return _readings[enc_num];
}

 float Encoders_GetDistance(uint8_t enc_num)
{
//	return ((float)_readings[enc_num]) * (float)ROBO_WHEELDIAMETER * PI / (float)ENC_PUL_PER_TURN;
	 return 0.f;
}

 float Encoders_GetAngle(uint8_t enc_num)
{
	return ((float)_readings[enc_num]) * ENC_ANG_PER_PULSE;
}

 void Encoders_Reset(uint8_t enc_num)
{
	_readings[enc_num] = 0;
}

 void Encoders_ResetAll()
{
	_readings[0] = 0;
	_readings[1] = 0;
	_readings[2] = 0;
	_readings[3] = 0;
}

static bool _encLastStates[4]; // true on high
 void Encoders_StartRead()
{
	EnableEncoders();
	// Check initial pin states on encoders
	_encLastStates[ENC_FL] = CheckPinHigh(ENC_Port_FL, ENC_Pin_FL);
	_encLastStates[ENC_FR] = CheckPinHigh(ENC_Port_FR, ENC_Pin_FR);
	_encLastStates[ENC_BL] = CheckPinHigh(ENC_Port_BL, ENC_Pin_BL);
	_encLastStates[ENC_BR] = CheckPinHigh(ENC_Port_BR, ENC_Pin_BR);

	_isEncReading = true;
}

 void Encoders_StopRead()
{
	_isEncReading = false;
	DisableEncoders();
}

 bool Encoders_IsReading()
{
	return _isEncReading;
}

void Encoders_Update(uint16_t interval)
{
	if( _encLastStates[ENC_FL] && CheckPinLow(ENC_Port_FL, ENC_Pin_FL) )
	{
		_readings[ENC_FL] += 1;
		_encLastStates[ENC_FL] = false;
	}
	//else if( !_encLastStates[ENC_FL] && CheckPinHigh(ENC_Port_FL, ENC_Pin_FL) )
	//{
		//_readings[ENC_FL] += 1;
	//	_encLastStates[ENC_FL] = true;
	//}
	if( _encLastStates[ENC_BR] && CheckPinLow(ENC_Port_BR, ENC_Pin_BR) )
	{
		_readings[ENC_BR] += 1;
		_encLastStates[ENC_BR] = false;
	}
	//else if( !_encLastStates[ENC_BR] && CheckPinHigh(ENC_Port_BR, ENC_Pin_BR) )
	//{
	//	_readings[ENC_BR] += 1;
	//	_encLastStates[ENC_FL] = true;
	//}
	if( _encLastStates[ENC_FR] && CheckPinLow(ENC_Port_FR, ENC_Pin_FR) )
	{
		_readings[ENC_FR] += 1;
		_encLastStates[ENC_FR] = false;
	}
	//else if( !_encLastStates[ENC_FR] && CheckPinHigh(ENC_Port_FR, ENC_Pin_FR) )
	//{
	//	_readings[ENC_FR] += 1;
	//	_encLastStates[ENC_FR] = true;
	//}
	if( _encLastStates[ENC_BL] && CheckPinLow(ENC_Port_BL, ENC_Pin_BL) )
	{
		_readings[ENC_BL] += 1;
		_encLastStates[ENC_BL] = false;
	}
	//else if( !_encLastStates[ENC_BL] && CheckPinHigh(ENC_Port_BL, ENC_Pin_BL) )
	//{
	//	_readings[ENC_BL] += 1;
	//	_encLastStates[ENC_BL] = true;
	//}

	_encLastStates[ENC_FL] = CheckPinHigh(ENC_Port_FL, ENC_Pin_FL);
	_encLastStates[ENC_FR] = CheckPinHigh(ENC_Port_FR, ENC_Pin_FR);
	_encLastStates[ENC_BL] = CheckPinHigh(ENC_Port_BL, ENC_Pin_BL);
	_encLastStates[ENC_BR] = CheckPinHigh(ENC_Port_BR, ENC_Pin_BR);
}
