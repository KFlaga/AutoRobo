/*
 * RoboMovementSensor.c
 *
 *  Created on: Mar 5, 2017
 *      Author: Kamil
 */


#include <movement/RoboMovementSensor.h>
#include <movement/RoboMovementControl.h>
#include <Utils.h>

#ifdef VECTOR3_MOVEMENT

#define MIN_ANGLE 0.001f

static MovementSensor _moveSensor;


void sensorSoftware_Init();
void sensorSoftware_Update(uint16_t us100);
void sensorSoftware_StopReadings();
void sensorSoftware_StartReadings();
void sensorSoftware_ResetPosition();
void sensorSoftware_ResetOrientation();


void SetSensorType(enum SensorType sensorType)
{
	switch(sensorType)
	{
	case SensorType_Software:
	default:
		_moveSensor.Init = sensorSoftware_Init;
		_moveSensor.Update = sensorSoftware_Update;
		_moveSensor.StartReadings = sensorSoftware_StartReadings;
		_moveSensor.StopReadings = sensorSoftware_StopReadings;
		_moveSensor.ResetOrientation = sensorSoftware_ResetOrientation;
		_moveSensor.ResetPosition = sensorSoftware_ResetPosition;
		break;
	}
}

MovementSensor* GetMovementSensor()
{
	return &_moveSensor;
}

//=====================================//
// SENSOR - SOFTWARE //
//=====================================//

static bool _readingsOn = false;

void sensorSoftware_Init()
{
	sensorSoftware_ResetPosition();
	sensorSoftware_ResetOrientation();
}

void sensorSoftware_Update(uint16_t us100)
{
	if(_readingsOn == true)
	{
		float dt_ms = 0.1f * (float)us100;
		_moveSensor.data.velocityLinear = RoboGetLinearVelocity();
		_moveSensor.data.velocityAngular = RoboGetAngularVelocity();

		// Lets assume that:
		// - since last update robot rotated by angle = v_ang * dt
		float da = dt_ms * _moveSensor.data.velocityAngular;
		// - so we need to update direction
		Vector3 newDir = Vector3_FromAngle_YX(_moveSensor.data.angle + da);
		// - ok now to find new position (or position change 's' = [sx,sy]):
		// --- let d = [dx, dy] be direction
		// --- dx = -sin(a), dy = cos(a)
		// --- sx = v_lin/v_ang * (cos(a+v_ang*dt) - cos(a)) * dt
		// --- sy = v_lin/v_ang * (sin(a+v_ang*dt) - sin(a)) * dt
		// --- if angle is less than 0.01 rad, then :
		// --- sx = -v_lin * dt * sin(a)
		// --- sy =  v_lin * dt * cos(a)

		float sx, sy;
		if(da < MIN_ANGLE)
		{
			float vt = _moveSensor.data.velocityLinear * dt_ms;
			sx = vt * _moveSensor.data.direction.x; // v * dt * (-sin(a))
			sy = vt * _moveSensor.data.direction.y; // v * dt * cos(a)
		}
		else
		{
			float vtw = _moveSensor.data.velocityLinear * dt_ms / _moveSensor.data.velocityAngular;
			sx = vtw * (cos(_moveSensor.data.angle + da) - _moveSensor.data.direction.y); // v/w * dt * (cos(a+wdt)-cos(a))
			sy = vtw * (sin(_moveSensor.data.angle + da) + _moveSensor.data.direction.x); // v/w * dt * (sin(a+wdt)+(-sin(a)))
		}

		_moveSensor.data.angle = _moveSensor.data.angle + da;
		_moveSensor.data.direction = newDir;
		_moveSensor.data.position.x = _moveSensor.data.position.x + sx;
		_moveSensor.data.position.y = _moveSensor.data.position.y + sy;
	}
}

void sensorSoftware_StopReadings()
{
	_readingsOn = false;
}

void sensorSoftware_StartReadings()
{
	_readingsOn = true;
}

void sensorSoftware_ResetPosition()
{
	_moveSensor.data.position = Vector3_Zero;
}

void sensorSoftware_ResetOrientation()
{
	_moveSensor.data.direction = Vector3_UnitY;
	_moveSensor.data.angle = 0.0f;
	//movementSensor.data.orientation = (Quaternion) { .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
}
#endif
