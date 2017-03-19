#ifndef ENCODERS_H_
#define ENCODERS_H_

#include <Utils.h>
#include "Pins.h"
#include "ModulesControl.h"

#define EnableEncoders() SetPin(ENC_Enable_Port, ENC_Enable_Pin)
#define DisableEncoders() ResetPin(ENC_Enable_Port, ENC_Enable_Pin)

#define ENC_FL 0
#define ENC_FR 1
#define ENC_BL 2
#define ENC_BR 3

#define ENC_ANG_PER_PULSE 11.25
#define ENC_PUL_PER_TURN 32
#define ENC_DIST_PER_PULSE (float)ROBO_WHEELDIAMETER * PI / (float)ENC_PUL_PER_TURN

 uint16_t Encoders_GetReadings(uint8_t enc_num); // get raw pulse count
 float Encoders_GetDistance(uint8_t enc_num); // get distance based on pulse count
 float Encoders_GetAngle(uint8_t enc_num); // get angle
 void Encoders_Reset(uint8_t enc_num);
 void Encoders_ResetAll();
 void Encoders_StartRead(); // Enables encoders
 void Encoders_StopRead();
 bool Encoders_IsReading();

void Encoders_Update(uint16_t);
void Encoders_Start();
void Encoders_Stop();

void Encoders_Init(Module*);

// regulation:
// count pulses -> every pulse = x [mm]
// if path have few pulse left -> slow down a little
// in f.eg. 2 motor pulses -> 66% and 33% of speed

#endif
