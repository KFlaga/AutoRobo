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

#define MOVE_FORWARD 0
#define MOVE_BACKWARDS 1

#define ROBO_VEL_MAX 172 // Max velocity in [mm/s]
#define ROBO_VEL_MAX_PER_MS 0.172f // Max velocity in [mm/ms]
#define ROBO_DEGS_MAX 184 // Max velocity in [deg/s]
#define ROBO_PULSE_COUNT_MAX 100
#define ROBO_PULSE_LEN 33
#define ROBO_WHEELRADIUS 39
#define ROBO_WHEELBASE 74 // perpendicular distance from center to wheel (in x axis)
#define ROBO_WHEELDIST 130 // distance from center to wheel
#define ROBO_WHEELDIAMETER 78
#define PI 3.1416
#define DEG_TO_RAD(angle_deg) ((angle_deg) * 0.01745f)
#define RAD_TO_DEG(angle_rad) ((angle_rad) * 57.296f)

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

#define MotoSetPulseHigh(port,pin) SetPin(port,pin)
#define MotoSetPulseLow(port,pin) ResetPin(port,pin)

#define MotoAllForward() MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL); \
	MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR); \
	MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL); \
	MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);

#define MotoAllBackward() MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL); \
	MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR); \
	MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL); \
	MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);

void InitMotorIO();

// Starts movement of motors
void MotoStartMove();
// Stops movement of motors (no more pulses)
void MotoEndMove();
// Sets length of motors pulses (pulse duty cycle [0-1])
void MotoUpdateSpeeds(float pulFL, float pulFR, float pulBL, float pulBR);
// Set length of one motor pulses (pulse duty cycle [0-1])
void MotoUpdateSpeed(uint8_t motor, float speed);

uint16_t* GetTargetPulses();
uint16_t GetCurrentPulse();
uint16_t GetPulseLength();

#endif
