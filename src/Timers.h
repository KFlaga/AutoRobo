#ifndef TIMERS_H_
#define TIMERS_H_

#include "stm32f4xx.h"

typedef void(*SysTickCallback)();

/*
 * Register function/counter to systick
 * Slots are 0 - 31
 * Registering f/c on used slot will result in overrite
 * unless nooverriting one is used
 * Unregistering will free slot
 * Returns 0 on success, < 0 on failure, > 0 additional info
 * NOASSERT version do not make any assertions
 */
 int8_t RegisterTickFunction(SysTickCallback callback, uint8_t slot);
 int8_t RegisterTickCounter(volatile uint32_t* counter, uint8_t slot);
 void UnregisterTickFunction(uint8_t slot);
 void UnregisterTickCounter(uint8_t slot);
 int8_t RegisterTickFunction_NOVWRT(SysTickCallback callback, uint8_t slot);
 int8_t RegisterTickCounter_NOVWRT(volatile uint32_t* counter, uint8_t slot);
 void RegisterTickFunction_NOASSERT(SysTickCallback callback, uint8_t slot);
 void RegisterTickCounter_NOASSERT(volatile  uint32_t* counter, uint8_t slot);


#define SLOT_MotoPWM 0

#define SLOT_RoboWait 4

#define SLOTC_LCDDelay 0
#define SLOTC_LCDWait 1

#define ERROR_SLOTHIGH -1
#define ERROR_NULLPTR -2
#define ERROR_OTHER -3
#define ERROR_SLOTUSED -4

#endif /* TIMERS_H_ */
