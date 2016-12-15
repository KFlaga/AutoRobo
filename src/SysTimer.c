
#include "stm32f4xx_it.h"
#include "Timers.h"

/*
 * Tables of functions/counters registered to
 * systick. Flags indicated if slot is registered.
 */
static SysTickCallback RegisteredFunctions[32];
static volatile uint32_t RF_Flags = 0;
static volatile uint32_t* RegisteredCounters[32];
static volatile uint32_t RC_Flags = 0;

/*
 * Tick of main sys timer
 * On every interrupt:
 *  - update registered counters
 *  - execute registered functions ( pointers )
 */
void SysTick_Handler()
{
	uint8_t i = 0;
	for(; i < 32; i++)
	{
		if(RF_Flags & 1<<i)
			(RegisteredFunctions[i])();
		if(RC_Flags & 1<<i)
			(*(RegisteredCounters[i])) += 1;
	}
}

__inline__ int8_t RegisterTickFunction(SysTickCallback callback, uint8_t slot)
{
	if(slot > 31)
		return ERROR_SLOTHIGH;
	if(callback == 0)
		return ERROR_NULLPTR;
	RegisteredFunctions[slot] = callback;
	RF_Flags |= 1 << slot;
	return 0;
}

__inline__ int8_t RegisterTickCounter(volatile uint32_t* counter, uint8_t slot)
{
	if(slot > 31)
		return ERROR_SLOTHIGH;
	if(counter == 0)
		return ERROR_NULLPTR;
	RegisteredCounters[slot] = counter;
	RC_Flags |= 1 << slot;
	return 0;
}

__inline__ void UnregisterTickFunction(uint8_t slot)
{
	RF_Flags &= ~((uint32_t)(1 << slot));
}

__inline__ void UnregisterTickCounter(uint8_t slot)
{
	RC_Flags &= ~((uint32_t)(1 << slot));
}

__inline__ int8_t RegisterTickFunction_NOVWRT(SysTickCallback callback, uint8_t slot)
{
	if(RF_Flags & 1 << slot)
		return ERROR_SLOTUSED;
	return RegisterTickFunction(callback, slot);
}

__inline__ int8_t RegisterTickCounter_NOVWRT(volatile uint32_t* counter, uint8_t slot)
{
	if(RC_Flags & 1 << slot)
		return ERROR_SLOTUSED;
	return RegisterTickCounter(counter, slot);
}

__inline__ void RegisterTickFunction_NOASSERT(SysTickCallback callback, uint8_t slot)
{
	RegisteredFunctions[slot] = callback;
	RF_Flags |= 1 << slot;
}

__inline__ void RegisterTickCounter_NOASSERT(volatile uint32_t* counter, uint8_t slot)
{
	RegisteredCounters[slot] = counter;
	RC_Flags |= 1 << slot;
}

