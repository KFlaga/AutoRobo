

#ifndef EXECINSTRMENU_H_
#define EXECINSTRMENU_H_

#include <movement/RoboProgram.h>
#include "MenuModule.h"

#define INST_STATUS_OK 0
#define INST_STATUS_BWD 1

void ChooseInstrMenu_Init(Menu* parent);
 Menu* ChooseInstrMenu_GetMenu();
 RoboInstruction* ChooseInstrMenu_GetInstr();

#endif /* EXECINSTRMENU_H_ */
