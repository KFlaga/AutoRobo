/*
 * RoboMovementSensor.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Kamil
 */

#ifndef MOVEMENT_ROBOMOVEMENTSENSOR_H_
#define MOVEMENT_ROBOMOVEMENTSENSOR_H_

#include "RoboMath.h"

// Movement of robot is in 3d
//#define VECTOR3_MOVEMENT

// Movement of robot is in 2d
#define VECTOR2_MOVEMENT

typedef void(*RoboMovementSensor_Init)();
typedef void(*RoboMovementSensor_Update)(unsigned short us100);
typedef void(*RoboMovementSensor_Reset)();
typedef void(*RoboMovementSensor_Readings)();

#ifdef VECTOR3_MOVEMENT
struct SensorData
{
	Vector3 position; // Current position of robot center
	Vector3 direction; // Current direction of local Y axis (robot facing direction)
	float angle; // Current angle of local Y axis (robot facing direction)
	float velocityLinear; // Linear velocity of robot in [mm/ms]
	float velocityAngular; // Angular velocity of robot in [rad/ms]. Positive rotates robot right.
};
#else
struct SensorData
{
	Vector2 position; // Current position of robot center
	Vector2 direction; // Current direction of local Y axis (robot facing direction)
	float angle; // Current angle of local Y axis (robot facing direction)
	float velocityLinear; // Linear velocity of robot in [mm/ms]
	float velocityAngular; // Angular velocity of robot in [rad/ms]. Positive rotates robot right.
};
#endif

/*
 * Interface for robot movement sensors handler.
 * Provides data about robot current velocity, position, orientation.
 */
typedef struct _RoboMovementSensor
{
	struct SensorData data;

	// Inits sensor system
	RoboMovementSensor_Init Init;

	// Updates measurements.
	// Parameter: time in 100us since last update
	RoboMovementSensor_Update Update;

	// Stops updating movement data
	RoboMovementSensor_Readings StopReadings;

	// Starts updating movement data
	RoboMovementSensor_Readings StartReadings;

	// Sets current robot position as origin (0,0,0)
	RoboMovementSensor_Reset ResetPosition;

	// Sets current orientation as base one (movement forward is along positive y axis)
	RoboMovementSensor_Reset ResetOrientation;

} MovementSensor;

enum SensorType
{
	// Velocities are based on max motors speed
	// Assumes that actual speed is proportional to duty cycle
	SensorType_Software,
	// Velocities are based on encoders attached to each wheel
	// (for now encoders are taken off and were a bit of disappointment,
	//  so I probably won't implement it)
	SensorType_Encoders,
	// Velocities are based on measurements from accelerometer and gyro
	// (for now I haven't even bought them, but intend in future)
	SensorType_AccelGyro
};

void SetSensorType(enum SensorType sensorType);
MovementSensor* GetMovementSensor();

#endif /* MOVEMENT_ROBOMOVEMENTSENSOR_H_ */
