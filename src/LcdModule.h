
#ifndef LCDMODULE_H_
#define LCDMODULE_H_

#include "Core.h"
#include "ModulesControl.h"

typedef void(*LCDFunction)();

void LCDWait(uint16_t ms, LCDFunction onContinue);
__inline__ void LcdTurnedOn();
__inline__ void LcdTurnedOff();
__inline__ bool LcdIsOn();
__inline__ bool LcdIsBusy();

void LcdModule_Init(Module* module);
void LcdModule_Update(uint16_t interval);
void LcdModule_Start();
void LcdModule_Stop();

__inline__ bool LcdModule_WriteLine(uint8_t line, uint8_t* data, uint8_t size);
__inline__ void LcdModule_WriteEnd();


#endif /* LCDMODULE_H_ */
