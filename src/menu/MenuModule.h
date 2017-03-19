/*
 * Base menu system, that handles navigation between children/parent menus
 * with buttons and have 1-2 lines of text to show
 */

#ifndef MENUBASE_H_
#define MENUBASE_H_

#include <Utils.h>
#include "ModulesControl.h"

typedef void(*MenuHandler)(uint16_t);

typedef struct _Menu
{
	struct _Menu* parent;
	struct _Menu* child;
	MenuHandler onReturn;

	unsigned char lineTop[17];
	unsigned char lineBot[17];

	MenuHandler onUpdate;
} Menu;


void MenuModule_Init(Module* module);
void MenuModule_Update(uint16_t interval);
void MenuModule_Start();
void MenuModule_Stop();

 void MenuModule_GoToChild(Menu* child, MenuHandler onReturn);
 void MenuModule_BackToParent(uint16_t status);

 void MenuModule_RefreshLcdTop();
 void MenuModule_RefreshLcdBot();

#endif /* MENUBASE_H_ */
