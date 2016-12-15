#include "MotorControl.h"
#include "stm32f4xx.h"
#include "Timers.h"

void MotoEndMove_FL();
void MotoEndMove_FR();
void MotoEndMove_BL();
void MotoEndMove_BR();

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

static volatile uint16_t remainingPulse[4];

void MotoMove(uint8_t pulFL, uint8_t pulFR, uint8_t pulBL, uint8_t pulBR)
{
	remainingPulse[0] = pulFL;
	remainingPulse[1] = pulFR;
	remainingPulse[2] = pulBL;
	remainingPulse[3] = pulBR;

	RegisterTickFunction_NOASSERT(MotoEndMove_FL, SLOT_MotoPWM_FL);
	RegisterTickFunction_NOASSERT(MotoEndMove_FR, SLOT_MotoPWM_FR);
	RegisterTickFunction_NOASSERT(MotoEndMove_BL, SLOT_MotoPWM_BL);
	RegisterTickFunction_NOASSERT(MotoEndMove_BR, SLOT_MotoPWM_BR);

	MotoBeginMove(MOTO_Port_FL, MOTO_Pin_PWM_FL);
	MotoBeginMove(MOTO_Port_FR, MOTO_Pin_PWM_FR);
	MotoBeginMove(MOTO_Port_BL, MOTO_Pin_PWM_BL);
	MotoBeginMove(MOTO_Port_BR, MOTO_Pin_PWM_BR);
}

void MotoEndMove_FL()
{
	if (remainingPulse[MOTO_FL] > 0)
		remainingPulse[MOTO_FL] -= 1;
	else
	{
		MotoEndMove(MOTO_Port_FL, MOTO_Pin_PWM_FL);
		UnregisterTickFunction(SLOT_MotoPWM_FL);
	}
}

void MotoEndMove_FR()
{
	if (remainingPulse[MOTO_FR] > 0)
		remainingPulse[MOTO_FR] -= 1;
	else
	{
		MotoEndMove(MOTO_Port_FR, MOTO_Pin_PWM_FR);
		UnregisterTickFunction(SLOT_MotoPWM_FR);
	}
}

void MotoEndMove_BL()
{
	if (remainingPulse[MOTO_BL] > 0)
		remainingPulse[MOTO_BL] -= 1;
	else
	{
		MotoEndMove(MOTO_Port_BL, MOTO_Pin_PWM_BL);
		UnregisterTickFunction(SLOT_MotoPWM_BL);
	}
}

void MotoEndMove_BR()
{
	if (remainingPulse[MOTO_BR] > 0)
		remainingPulse[MOTO_BR] -= 1;
	else
	{
		MotoEndMove(MOTO_Port_BR, MOTO_Pin_PWM_BR);
		UnregisterTickFunction(SLOT_MotoPWM_BR);
	}
}

void MotoBreak(uint8_t motonum)
{
	/*
	if (motonum & MOTO_FL)
	{
		MotoEndMove(MOTO_Port_FL, MOTO_Pin_PWM_FL);
		remainingPulse[0] = 0;
		UnregisterTickFunction(SLOT_MotoPWM_FL);
		MotoSuspend(MOTO_Port_FL, MPin_Fwd_FL, MPin_Bwd_FL);
	}
	else if (motonum & MOTO_FR)
	{
		MotoEndMove(MOTO_Port_FR, MOTO_Pin_PWM_FR);
		remainingPulse[1] = 0;
		UnregisterTickFunction(SLOT_MotoPWM_FR);
		MotoSuspend(MPort_FR, MPin_Fwd_FR, MPin_Bwd_FR);
	}
	else if (motonum & MOTO_BL)
	{
		MotoEndMove(MPort_BL, MPin_PWM_BL);
		remainingPulse[2] = 0;
		UnregisterTickFunction(SLOT_MotoPWM_BL);
		MotoSuspend(MPort_BL, MPin_Fwd_BL, MPin_Bwd_BL);
	}
	else if (motonum & MOTO_BR)
	{
		MotoEndMove(MPort_BR, MPin_PWM_BR);
		remainingPulse[3] = 0;
		UnregisterTickFunction(SLOT_MotoPWM_BR);
		MotoSuspend(MPort_BR, MPin_Fwd_BR, MPin_Bwd_BR);
	}
	*/
}
