
/*
 * High level functions to move robot
 * ( to include only in RoboModule.c / RoboProgram.c
 *   -> works kinda like private section
 *   declarations of RoboModule with friend RoboProgram
 *   to call directly only in RoboModule.c )
 */

#ifndef ROBOMOVE_H_
#define ROBOMOVE_H_

#include "Core.h"
#include "CallTree.h"

// Experimental values to have 180deg rotate on full speed when such params are set
// #define ROBO_WHEELBASE 173 // 71 // in mm rozstaw osi
#define ROBO_WHEELRAD 36

// x - unused, distance in mm, speed in % of max speed
void RoboMoveFwd(uint16_t distance, uint16_t speed, uint16_t);
void RoboMoveBwd(uint16_t distance, uint16_t speed, uint16_t);
void RoboWait(uint16_t time_ms, uint16_t, uint16_t);
void RoboRotateLeft(uint16_t angle, uint16_t speed, uint16_t);
void RoboRotateRight(uint16_t angle, uint16_t speed, uint16_t);
void RoboArcFwdLeft(uint16_t radius, uint16_t arclen, uint16_t speed);
void RoboArcFwdRight(uint16_t radius, uint16_t arclen, uint16_t speed);
void RoboArcBwdLeft(uint16_t radius, uint16_t arclen, uint16_t speed);
void RoboArcBwdRight(uint16_t radius, uint16_t arclen, uint16_t speed);


// Turn left (CCW) right (CW) moving forward -> angle in degrees
// angspeed and linspeed determines how fast it will turn
// or move forward. angspeed+linspeed <= 100
// If a+l > 100 lower it proportionally
// ( turn radius remains the same )
//void RoboTurnLeftDegFwd(uint16_t angle, uint8_t angspeed, uint8_t linspeed);
//void RoboTurnRightDegFwd(uint16_t angle, uint8_t angspeed, uint8_t linspeed);
//void RoboTurnLeftDegBwd(uint16_t angle, uint8_t angspeed, uint8_t linspeed);
//void RoboTurnRightDegBwd(uint16_t angle, uint8_t angspeed, uint8_t linspeed);

// Make define
//void RoboTurnLeftDegFwd(uint16_t angle, uint16_t radius, uint8_t speed);
//void RoboTurnRightDegFwd(uint16_t angle, uint16_t radius, uint8_t speed);

// Make define - rotate - 0 linspeed
//void RoboRotateLeft(uint16_t angle, uint8_t aspeed);
//void RoboRotateRight(uint16_t angle, uint8_t aspeed);

// Move robot to cover dist in time of 'time' [ms]
// If not possible move as fast as possible
//void RoboMoveFwdTime(uint16_t dist, uint16_t time);
// In this version move robot with speed for time ms
//void RoboMoveFwdTimeVel(uint16_t time, uint8_t speed);



#endif /* ROBOMOVE_H_ */
