/*
 * RoboMotorControl.h
 *
 *  Created on: Mar 5, 2017
 *      Author: Kamil
 */

#ifndef MOVEMENT_ROBOMOVEMENTCONTROL_H_
#define MOVEMENT_ROBOMOVEMENTCONTROL_H_

#include <movement/LowLevelMotorControl.h>

// Enables movement of wheels.
// Turns motor pulse timer on
void RoboStartMove();

// Disables movement of wheels.
// Turns motor pulse timer off
void RoboStopMove();

// Sets direction (fwd/bwd) of movement for one wheel
void RoboSetWheelDirection(uint8_t wheel, int8_t direction);

// Sets speed (part of max speed) for one wheel
void RoboSetWheelSpeed(uint8_t wheel, float speed);

// Sets linear and angular velocity for robot.
// Computes and sets speeds for each wheel.
// Speeds are in mm per ms and radians per ms
void RoboSetVelocity(float linearVelocity, float angularVelocity);

// Returns velocity set last time in RoboSetVelocity() [mm/ms]
float RoboGetLinearVelocity();
// Returns velocity set last time in RoboSetVelocity() [rad/ms]
float RoboGetAngularVelocity();

#endif /* MOVEMENT_ROBOMOVEMENTCONTROL_H_ */
