#include <movement/LowLevelMotorControl.h>
#include "stm32f4xx.h"
#include "Timers.h"

void motoUpdate();

void InitMotorIO()
{
	// Init pins: PC->8,9,10,11,12,5,6 PB->8,9,12 PD->2 PA->0,1,4
	// Reset and set mode register -> all outputs
	InitPinOutput(MOTO_Port_FL, MOTO_PinNum_PWM_FL);
	InitPinOutput(MOTO_Port_FL, MOTO_PinNum_IN1_FL);
	InitPinOutput(MOTO_Port_FL, MOTO_PinNum_IN2_FL);
	InitPinOutput(MOTO_Port_FR, MOTO_PinNum_PWM_FR);
	InitPinOutput(MOTO_Port_FR, MOTO_PinNum_IN1_FR);
	InitPinOutput(MOTO_Port_FR, MOTO_PinNum_IN2_FR);
	InitPinOutput(MOTO_Port_BL, MOTO_PinNum_PWM_BL);
	InitPinOutput(MOTO_Port_BL, MOTO_PinNum_IN1_BL);
	InitPinOutput(MOTO_Port_BL, MOTO_PinNum_IN2_BL);
	InitPinOutput(MOTO_Port_BR, MOTO_PinNum_PWM_BR);
	InitPinOutput(MOTO_Port_BR, MOTO_PinNum_IN1_BR);
	InitPinOutput(MOTO_Port_BR, MOTO_PinNum_IN2_BR);
	InitPinOutput(MotoPort_Stby_L, MOTO_PinNum_Stby_L);
	InitPinOutput(MotoPort_Stby_R, MOTO_PinNum_Stby_R);
	// Set open drain on all pins ( pull-down in bridge )
	InitPinPUPD(MOTO_Port_FL, MOTO_PinNum_PWM_FL);
	InitPinPUPD(MOTO_Port_FL, MOTO_PinNum_IN1_FL);
	InitPinPUPD(MOTO_Port_FL, MOTO_PinNum_IN2_FL);
	InitPinPUPD(MOTO_Port_FR, MOTO_PinNum_PWM_FR);
	InitPinPUPD(MOTO_Port_FR, MOTO_PinNum_IN1_FR);
	InitPinPUPD(MOTO_Port_FR, MOTO_PinNum_IN2_FR);
	InitPinPUPD(MOTO_Port_BL, MOTO_PinNum_PWM_BL);
	InitPinPUPD(MOTO_Port_BL, MOTO_PinNum_IN1_BL);
	InitPinPUPD(MOTO_Port_BL, MOTO_PinNum_IN2_BL);
	InitPinPUPD(MOTO_Port_BR, MOTO_PinNum_PWM_BR);
	InitPinPUPD(MOTO_Port_BR, MOTO_PinNum_IN1_BR);
	InitPinPUPD(MOTO_Port_BR, MOTO_PinNum_IN2_BR);
	InitPinPUPD(MotoPort_Stby_L, MOTO_PinNum_Stby_L);
	InitPinPUPD(MotoPort_Stby_R, MOTO_PinNum_Stby_R);
	// Reset and set speeds to normal speed
	InitPinMedSpeed(MOTO_Port_FL, MOTO_PinNum_PWM_FL);
	InitPinMedSpeed(MOTO_Port_FL, MOTO_PinNum_IN1_FL);
	InitPinMedSpeed(MOTO_Port_FL, MOTO_PinNum_IN2_FL);
	InitPinMedSpeed(MOTO_Port_FR, MOTO_PinNum_PWM_FR);
	InitPinMedSpeed(MOTO_Port_FR, MOTO_PinNum_IN1_FR);
	InitPinMedSpeed(MOTO_Port_FR, MOTO_PinNum_IN2_FR);
	InitPinMedSpeed(MOTO_Port_BL, MOTO_PinNum_PWM_BL);
	InitPinMedSpeed(MOTO_Port_BL, MOTO_PinNum_IN1_BL);
	InitPinMedSpeed(MOTO_Port_BL, MOTO_PinNum_IN2_BL);
	InitPinMedSpeed(MOTO_Port_BR, MOTO_PinNum_PWM_BR);
	InitPinMedSpeed(MOTO_Port_BR, MOTO_PinNum_IN1_BR);
	InitPinMedSpeed(MOTO_Port_BR, MOTO_PinNum_IN2_BR);
	InitPinMedSpeed(MotoPort_Stby_L, MOTO_PinNum_Stby_L);
	InitPinMedSpeed(MotoPort_Stby_R, MOTO_PinNum_Stby_R);
	// Reset pull-up / pull-down
	InitPinPullUp(MOTO_Port_FL, MOTO_PinNum_PWM_FL);
	InitPinPullUp(MOTO_Port_FL, MOTO_PinNum_IN1_FL);
	InitPinPullUp(MOTO_Port_FL, MOTO_PinNum_IN2_FL);
	InitPinPullUp(MOTO_Port_FR, MOTO_PinNum_PWM_FR);
	InitPinPullUp(MOTO_Port_FR, MOTO_PinNum_IN1_FR);
	InitPinPullUp(MOTO_Port_FR, MOTO_PinNum_IN2_FR);
	InitPinPullUp(MOTO_Port_BL, MOTO_PinNum_PWM_BL);
	InitPinPullUp(MOTO_Port_BL, MOTO_PinNum_IN1_BL);
	InitPinPullUp(MOTO_Port_BL, MOTO_PinNum_IN2_BL);
	InitPinPullUp(MOTO_Port_BR, MOTO_PinNum_PWM_BR);
	InitPinPullUp(MOTO_Port_BR, MOTO_PinNum_IN1_BR);
	InitPinPullUp(MOTO_Port_BR, MOTO_PinNum_IN2_BR);
	InitPinPullUp(MotoPort_Stby_L, MOTO_PinNum_Stby_L);
	InitPinPullUp(MotoPort_Stby_R, MOTO_PinNum_Stby_R);
}

// Lengths of high pulses in ms for each motor
static volatile uint16_t targetHighPulses[4];
// Time that passed for current pulse
static volatile uint16_t currentPulse;
// Length of full pulse
static const uint16_t pulseLength = 33;
static const float pulseLengthF = 33.f;

uint16_t* GetTargetPulses()
{
	return targetHighPulses;
}

uint16_t GetCurrentPulse()
{
	return currentPulse;
}

uint16_t GetPulseLength()
{
	return pulseLength;
}

void MotoStartMove()
{
	targetHighPulses[0] = 0;
	targetHighPulses[1] = 0;
	targetHighPulses[2] = 0;
	targetHighPulses[3] = 0;

	RegisterTickFunction_NOASSERT(motoUpdate, SLOT_MotoPWM);

	currentPulse = 0;

	motoUpdate();
}

void MotoEndMove()
{
	UnregisterTickFunction(SLOT_MotoPWM);
	MotoSetPulseLow(MOTO_Port_FL, MOTO_Pin_PWM_FL);
	MotoSetPulseLow(MOTO_Port_FR, MOTO_Pin_PWM_FR);
	MotoSetPulseLow(MOTO_Port_BL, MOTO_Pin_PWM_BL);
	MotoSetPulseLow(MOTO_Port_BR, MOTO_Pin_PWM_BR);
}

void MotoUpdateSpeeds(float pulFL, float pulFR, float pulBL, float pulBR)
{
	targetHighPulses[0] = (uint16_t)(pulFL * pulseLengthF);
	targetHighPulses[1] = (uint16_t)(pulFR * pulseLengthF);
	targetHighPulses[2] = (uint16_t)(pulBL * pulseLengthF);
	targetHighPulses[3] = (uint16_t)(pulBR * pulseLengthF);
}

void MotoUpdateSpeed(uint8_t motor, float speed)
{
	targetHighPulses[motor] = (uint16_t)(speed * pulseLengthF);
}


void motoUpdate()
{
	if(currentPulse == 0)
	{
		if(targetHighPulses[MOTO_FL] > 0)
			MotoSetPulseHigh(MOTO_Port_FL, MOTO_Pin_PWM_FL);
		if(targetHighPulses[MOTO_FR] > 0)
			MotoSetPulseHigh(MOTO_Port_FR, MOTO_Pin_PWM_FR);
		if(targetHighPulses[MOTO_BL] > 0)
			MotoSetPulseHigh(MOTO_Port_BL, MOTO_Pin_PWM_BL);
		if(targetHighPulses[MOTO_BR] > 0)
			MotoSetPulseHigh(MOTO_Port_BR, MOTO_Pin_PWM_BR);
	}

	if(currentPulse == targetHighPulses[MOTO_FL])
		MotoSetPulseLow(MOTO_Port_FL, MOTO_Pin_PWM_FL);
	if(currentPulse == targetHighPulses[MOTO_FR])
		MotoSetPulseLow(MOTO_Port_FR, MOTO_Pin_PWM_FR);
	if(currentPulse == targetHighPulses[MOTO_BL])
		MotoSetPulseLow(MOTO_Port_BL, MOTO_Pin_PWM_BL);
	if(currentPulse == targetHighPulses[MOTO_BR])
		MotoSetPulseLow(MOTO_Port_BR, MOTO_Pin_PWM_BR);

	currentPulse = currentPulse + 1;
	if(currentPulse > pulseLength)
		currentPulse = 0;
}
