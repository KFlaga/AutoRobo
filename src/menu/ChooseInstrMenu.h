

#ifndef EXECINSTRMENU_H_
#define EXECINSTRMENU_H_

#include "MenuModule.h"
#include "../robo/RoboProgram.h"

#define INST_STATUS_OK 0
#define INST_STATUS_BWD 1

void ChooseInstrMenu_Init(Menu* parent);
__inline__ Menu* ChooseInstrMenu_GetMenu();
__inline__ RoboInstruction* ChooseInstrMenu_GetInstr();

#endif /* EXECINSTRMENU_H_ */
