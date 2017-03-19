#ifndef ROBOPROGRAM_H_
#define ROBOPROGRAM_H_

#include <Utils.h>
#include "RoboMath.h"

// Function prototype for all movement functions
typedef void(*RoboFunction)(uint16_t, uint16_t, uint16_t);
typedef uint8_t ExeFlag;

enum RoboFunctionType
{
	RoboFunction_Null = 0x0,
	RoboFunction_Wait = 0x1, // Instruction is wait-function
	RoboFunction_Movement = 0x200, // Instruction is movement-function
	RoboFunction_MoveLine = RoboFunction_Movement | 0x2,
	RoboFunction_Rotate = RoboFunction_Movement | 0x3,
	RoboFunction_MoveArc = RoboFunction_Movement | 0x4,
	RoboFunction_MoveToPoint = RoboFunction_Movement | 0x5,
	RoboFunction_ResetSensors = 0x6,

	RoboFunction_FunctionsCount = 7,
};

typedef struct _RoboFunDef
{
	uint16_t function; // RoboFuntionType
	uint8_t steering; // movement algorithm used
	uchar name[9]; // Short name of function to display
	uint8_t paramCount; // number of parameters used
	uchar paramNames[3]; // Names of parameters to display
	uchar paramUnits[6]; // Units of parameters to display
	uint16_t paramDefaultVals[3]; // Default values of parameters
	uint8_t paramIncrement[3]; // Change in parameters value for one button-press
} RoboFunDef;

// Single instruction to be executed in program
typedef struct _RoboInstruction
{
	const RoboFunDef* functionDef;
	uint16_t params[3]; // Values of parameters for function

	struct _RoboInstruction* next;
	struct _RoboInstruction* prev;
} RoboInstruction;

// Program containing multiple instruction
typedef struct _RoboProgram
{
	uchar name[13]; // max len = 12

	uint8_t instCount;
	RoboInstruction* firstInstruction;
	RoboInstruction* lastInstruction;
} RoboProgram;

const RoboFunDef* GetRoboFunctionDefinition(enum RoboFunctionType funId);

#endif
