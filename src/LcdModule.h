
#ifndef LCDMODULE_H_
#define LCDMODULE_H_

#include <Utils.h>
#include "ModulesControl.h"

typedef void(*LCDFunction)();

void LCDWait(uint16_t ms, LCDFunction onContinue);
 void LcdTurnedOn();
 void LcdTurnedOff();
 bool LcdIsOn();
 bool LcdIsBusy();

void LcdModule_Init(Module* module);
void LcdModule_Update(uint16_t interval);
void LcdModule_Start();
void LcdModule_Stop();

 bool LcdModule_WriteLine(uint8_t line, uint8_t* data, uint8_t size);
 void LcdModule_WriteEnd();


#endif /* LCDMODULE_H_ */
