#ifndef BUTTINS_H_
#define BUTTINS_H_

#include "Core.h"
#include "Pins.h"
#include "ModulesControl.h"

/*
 * Buttons codes:
 * eh a0 a1 : 0bxxx
 */
#define BUT_Up 4 // 0b100
#define BUT_Down 1 // 0b001
#define BUT_Left 5 // 0b101
#define BUT_Right 0 // 0b000
#define BUT_Fwd 3 // 0b011
#define BUT_Bwd 2 // 0b010
#define BUT_Switch1 6 // 0b110
#define BUT_Switch2 7 // 0b111

#define BUT_A0(code) code & 1
#define BUT_A1(code) code & 2
#define BUT_E(code) code & 4

typedef bool ButState;
#define BUTSTATE_PRESSED true
#define BUTSTATE_RELEASED false

#define BUT_WAS_PRESSED(oldstate, newstate) ((oldstate) == BUTSTATE_RELEASED) \
		&& ((newstate) == BUTSTATE_PRESSED)
#define BUT_WAS_RELEASED(oldstate, newstate) ((oldstate) == BUTSTATE_PRESSED) \
		&& ((newstate) == BUTSTATE_RELEASED)

// Checking buttons:
// Set: [e1,a0,a1] = [0,0,0] -> 1st button
// Check: high = not pressed, low = pressed
// Compare with previous state
// if(was high is low) -> button pressed (sw turned ON)
// else if(was low is high) -> button released (sw turned OFF)

// If button changed state notify listeners

//typedef void(*ButtonListener)(uint8_t buttonId);

//bool ButtonModule_RegisterButtonListner(ButtonListener);
//bool ButtonModule_UnregisterButtonListener(ButtonListener);

void ButtonModule_Init(Module*);
void ButtonModule_Update(uint16_t interval);
void ButtonModule_Start();
void ButtonModule_Stop();

void ButtonModule_Test();
void ButtonModule_Test1();

// rather than notifying keep flags open for everyone
// - current state
// - buttons pressed
// - buttons released

__inline__ uint16_t GetButtonStates();
__inline__ uint16_t GetPressedButtons();
__inline__ uint16_t GetReleasedButtons();

#endif
