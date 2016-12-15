/*
 * Interface to control motors directly
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include "Pins.h"

#define MOTO_FL 0
#define MOTO_FR 1
#define MOTO_BL 2
#define MOTO_BR 3

#define MotoForward(port, fwdpin, bwdpin) SetPin((port),(bwdpin));\
	ResetPin((port),(fwdpin))

#define MotoSuspend(port, fwdpin, bwdpin) SetPin((port),(fwdpin));\
	SetPin((port),(bwdpin))

#define MotoBackward(port, fwdpin, bwdpin) SetPin((port),(fwdpin));\
	ResetPin((port),(bwdpin))

#define MotoStop(port, fwdpin, bwdpin) ResetPin((port),(fwdpin));\
	ResetPin((port),(bwdpin))

#define MotoStandby() ResetPin(MotoPort_Stby_L,MotoPin_Stby_L); ResetPin(MotoPort_Stby_R,MotoPin_Stby_R)
#define MotoResume()	SetPin(MotoPort_Stby_L,MotoPin_Stby_L); SetPin(MotoPort_Stby_R,MotoPin_Stby_R)
#define CheckMotoStandby() CheckPinLow(MotoPort_Stby_L, MotopPin_Stby_L) || CheckPinLow(MotoPort_Stby_R, MotopPin_Stby_R)

#define MotoBeginMove(port,pin) SetPin(port,pin)
#define MotoEndMove(port,pin) ResetPin(port,pin)

#define MotoAllForward() MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL); \
	MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR); \
	MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL); \
	MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);

#define MotoAllBackward() MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL); \
	MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR); \
	MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL); \
	MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);


void InitMotorIO();

void MotoMove(uint8_t pulFL, uint8_t pulFR, uint8_t pulBL, uint8_t pulBR);
void MotoBreak(uint8_t motonum);

#endif
