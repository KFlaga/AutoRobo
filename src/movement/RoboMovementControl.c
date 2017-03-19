/*
 * RoboMovementControl.c
 *
 *  Created on: Mar 6, 2017
 *      Author: Kamil
 */

#include <movement/RoboMovementControl.h>
#include "RoboMath.h"


void RoboStartMove()
{
	MotoStartMove();
}

void RoboStopMove()
{
	MotoEndMove();
}

void RoboSetWheelDirection(uint8_t wheel, int8_t direction)
{
	switch(wheel)
	{
	case MOTO_FL:
		if(direction == MOVE_FORWARD)
		{MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);}
		else
		{MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);}
		break;
	case MOTO_FR:
		if(direction == MOVE_FORWARD)
		{MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);}
		else
		{MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);}
		break;
	case MOTO_BL:
		if(direction == MOVE_FORWARD)
		{MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);}
		else
		{MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);}
		break;
	case MOTO_BR:
		if(direction == MOVE_FORWARD)
		{MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);}
		else
		{MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);}
		break;
	default:
		break;
	}
}

void RoboSetWheelSpeed(uint8_t wheel, float speed)
{
	MotoUpdateSpeed(wheel, speed);
}

static float velocityLinear;
static float velocityAngular;

void RoboSetVelocity(float v_lin, float v_ang)
{
	velocityLinear = v_lin;
	velocityAngular = v_ang;
	// v_lin = v_left + v_right / 2
	// v_ang = (v_right - v_left) / 2
	// If linear velocity is negative then we move backwards
	// If angular velocity is positive then we turn left, if negative we turn right
	// From above we have:
	// v_right = v_lin + v_ang (clamped to [-1,1])
	// v_left = v_lin - v_ang (clamped to [-1,1])
	float v_right = v_lin + v_ang;
	float v_left = v_lin - v_ang;

	if(v_right < 0.f)
	{
		v_right = -v_right;
		MotoBackward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoBackward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}
	else
	{
		MotoForward(MOTO_Port_FR, MOTO_Pin_IN1_FR, MOTO_Pin_IN2_FR);
		MotoForward(MOTO_Port_BR, MOTO_Pin_IN1_BR, MOTO_Pin_IN2_BR);
	}

	if(v_left < 0.f)
	{
		v_left = -v_left;
		MotoBackward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoBackward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}
	else
	{
		MotoForward(MOTO_Port_FL, MOTO_Pin_IN1_FL, MOTO_Pin_IN2_FL);
		MotoForward(MOTO_Port_BL, MOTO_Pin_IN1_BL, MOTO_Pin_IN2_BL);
	}

	v_left = roboMin(v_left, 1.f);
	v_right = roboMin(v_right, 1.f);

	MotoUpdateSpeeds(v_left, v_right, v_left, v_right);
}

float RoboGetLinearVelocity()
{
	return velocityLinear;
}

float RoboGetAngularVelocity()
{
	return velocityAngular;
}
