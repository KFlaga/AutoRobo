#ifndef ROBOPROGRAM_H_
#define ROBOPROGRAM_H_

#include "Core.h"

// Function prototype for all movement functions
typedef void(*RoboFunction)(uint16_t, uint16_t, uint16_t);
typedef uint8_t ExeFlag;

/*typedef  struct _RoboParamDef
{
	uchar name[2];
	uchar unit[2];
	uint16_t defVal;
	uint16_t maxVal;
	uint8_t increment;
} RoboParamDef;*/

typedef struct _RoboFunDef
{
	RoboFunction function;
	uchar name[9];
	uint8_t paramCount;
//	RoboParamDef* paramDefs[3];
	uchar paramNames[3];
	uchar paramUnits[6];
	uint16_t paramDefaultVals[3];
	uint8_t paramIncrement[3];
} RoboFunDef;

// Single instruction to be executed in program
typedef struct _RoboInstruction
{
	const RoboFunDef* functionDef; // Id of robofunction
	uint16_t params[3];

	struct _RoboInstruction* next;
	struct _RoboInstruction* prev;
} RoboInstruction;

// Program containing multiple instruction
typedef struct _RoboProgram
{
	uchar name[13]; // max len = 12

	RoboInstruction* firstInstruction;
	RoboInstruction* lastInstruction;
	uint8_t instCount;

	RoboInstruction* currentInstruction;
	uint8_t curInstNum;
} RoboProgram;

__inline__ const RoboFunDef* GetRoboFunctionDefinition(uint8_t funId);

#define ExecuteRoboFunction(instruction) ((instruction)->functionDef->function)\
		((instruction)->params[0], (instruction)->params[1], (instruction)->params[2]);

#define ROBOFUN_COUNT 10

#define ROGOFUN_NULL 0
#define ROBOFUN_MOVFWD 1
#define ROBOFUN_MOVBWD 2
#define ROBOFUN_ROTLEFT 3
#define ROBOFUN_ROTRIGHT 4
#define ROBOFUN_ARCFWDLEFT 5
#define ROBOFUN_ARCFWDRIGHT 6
#define ROBOFUN_ARCBWDLEFT 7
#define ROBOFUN_ARCBWDRIGHT 8
#define ROBOFUN_WAIT 9



#endif
