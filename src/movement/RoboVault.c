#include <movement/RoboVault.h>

/*
 * Program table
 */
static RoboProgram* _programVault[VAULT_CAPACITY];
static uint8_t _programCount;

void _addStdPrograms();

void RoboVault_Init()
{
	_programCount = 0;
	_addStdPrograms();
}

RoboProgram* RoboVault_GetProgramById(uint8_t progId)
{
	return _programVault[progId];
}

RoboProgram* RoboVault_GetProgramByName(uchar* progName)
{
	int i;
	for(i = 0; i < _programCount; i++)
	{
		if(comp_mem(_programVault[i]->name,progName,12))
		{
			return _programVault[i];
		}
	}
	return 0;
}

RoboProgram** RoboVault_GetProgramList()
{
	return _programVault;
}

uint8_t RoboVault_GetProgramCount()
{
	return _programCount;
}

bool RoboVault_AddProgram(RoboProgram* prog)
{
	if(_programCount >= VAULT_CAPACITY)
		return false;

	_programVault[_programCount] = prog;
	_programCount++;
	return true;
}

bool RoboVault_RemoveProgram(RoboProgram* prog)
{
	int i;
	for(i = 0; i < _programCount; i++)
	{
		if(_programVault[i] == prog)
		{
			for(;i < _programCount-1; i++)
				_programVault[i] = _programVault[i+1];
			return true;
		}
	}
	return false;
}

#include "Memory.h"

void _addStdPrograms()
{
	// 1) wait 2s, go 1m forward, turn left 360 deg, go 1m forward, turn 360 right

	RoboProgram* prog1 = allocMemory(sizeof(RoboProgram));
	*prog1 = (RoboProgram) {
		.name = { 'f','w','d',' ','1','8','0',' ','l','o','o','p' },
				.instCount = 5
	};
	// a) alloc instr
	RoboInstruction* iwait = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* ifwd1 = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* irot1 = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* ifwd2 = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* irot2 = allocMemory(sizeof(RoboInstruction));

	// b) fill instructions
	*iwait = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_Wait),
				.next = ifwd1,
				.prev = 0,
				.params = { 2000, 0, 0 }
	};
	*ifwd1 = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveLine),
				.next = irot1,
				.prev = iwait,
				.params = { 1000, 100, 0 }
	};
	*irot1 = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_Rotate),
				.next = ifwd2,
				.prev = ifwd1,
				.params = { 180, 100, 0 }
	};
	*ifwd2 = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveLine),
				.next = irot2,
				.prev = irot1,
				.params = { 1000, 100, 0 }
	};
	*irot2 = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_Rotate),
				.next = 0,
				.prev = ifwd2,
				.params = { 180, 100, 0 }
	};

	// c) save to program
	prog1->firstInstruction = iwait;
	prog1->lastInstruction = irot2;

	// d) save program
	RoboVault_AddProgram(prog1);

	// 2) wait 2s, go 50 cm forward, go backward arc to right 90deg 50cm, go forward arc to left 90 deg 50cm, go backward 50cm, turn 180 deg
	RoboProgram* prog2 = allocMemory(sizeof(RoboProgram));
		*prog2 = (RoboProgram) {
			.name = { 'a','r','c','s',' ',' ',' ',' ',' ',' ',' ',' ' },
			.instCount = 6
		};

	RoboInstruction* iwait0 = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* ifwd = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* iarcbr = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* iarcfl = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* ibwd = allocMemory(sizeof(RoboInstruction));
	RoboInstruction* irot = allocMemory(sizeof(RoboInstruction));

	*iwait0 = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_Wait),
				.next = ifwd,
				.prev = 0,
				.params = { 2000, 0, 0 }
	};
	*ifwd = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveLine),
				.next = iarcbr,
				.prev = iwait0,
				.params = { 500, 100, 0 }
	};
	*iarcbr = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveArc),
				.next = iarcfl,
				.prev = ifwd,
				.params = { 500, 90, 100 }
	};
	*iarcfl = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveArc),
				.next = ibwd,
				.prev = iarcbr,
				.params = { 500, 90, 100 }
	};
	*ibwd = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_MoveLine),
				.next = irot,
				.prev = iarcfl,
				.params = { 500, 100, 0 }
	};
	*irot = (RoboInstruction) {
		.functionDef = GetRoboFunctionDefinition(RoboFunction_Rotate),
				.next = 0,
				.prev = ibwd,
				.params = { 180, 100, 0 }
	};

	prog2->firstInstruction = iwait0;
	prog2->lastInstruction = irot;

	RoboVault_AddProgram(prog2);

}
