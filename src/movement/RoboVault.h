/*
 * ProgramVault.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kamil
 */

#ifndef PROGRAMVAULT_H_
#define PROGRAMVAULT_H_

#include <movement/RoboProgram.h>
#include <Utils.h>

#define VAULT_CAPACITY 100

// Program vault saves programs
void RoboVault_Init();

RoboProgram* RoboVault_GetProgramById(uint8_t progId); // Id is position in list
RoboProgram* RoboVault_GetProgramByName(uint8_t* progName); // name ends on 0

RoboProgram** RoboVault_GetProgramList();
uint8_t RoboVault_GetProgramCount();

bool RoboVault_AddProgram(RoboProgram*);
bool RoboVault_RemoveProgram(RoboProgram*);

#endif /* PROGRAMVAULT_H_ */
