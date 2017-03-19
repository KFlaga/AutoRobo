#include <movement/RoboModule.h>
#include <movement/RoboProgram.h>
#include <movement/RoboMovementAlgorithm.h>
#include "Timers.h"

const RoboFunDef _roboFuns[RoboFunction_FunctionsCount] __attribute__ ((section(".rodata"))) = {
		(RoboFunDef) {
			.function = RoboFunction_Null,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 0,
					.name = { 'N','U','L','L',' ',' ',' ',' ', 0 },
					.paramNames =  { ' ',' ',' ' },
					.paramUnits = { ' ',' ',' ',' ',' ',' ' },
					.paramDefaultVals = { 0,0,0 },
					.paramIncrement = { 0,0,0},
		},
		(RoboFunDef) {
			.function = RoboFunction_MoveLine,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 2,
					.name = { 'M','o','v','e','L','i','n','e', 0 },
					.paramNames =  { 's','v',' ' }, // dist, vel,
					.paramUnits = { 'm','m','%',' ', 0, 0 },
					.paramDefaultVals = { 100, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboFunction_Rotate,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 2,
					.name = { 'R','o','t','a','t','e',' ',' ', 0 },
					.paramNames =  { 'A', 'w', 0 }, // angle, speed
					.paramUnits = { 'd','g', '%', ' ', 0, 0 },
					.paramDefaultVals = { 90, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboFunction_MoveArc,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 3,
					.name = { 'M','o','v','e','A','r','c',' ', 0 },
					.paramNames =  { 'R', 'A', 'w' }, // radius, angle, speed
					.paramUnits = { 'm','m', 'd', 'g', '%', ' ' },
					.paramDefaultVals = { 100, 90, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboFunction_MoveToPoint,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 3,
					.name = { 'M','o','v','e','P','n','t',' ', 0 },
					.paramNames =  { 'x', 'y', 'v' }, // x-pos, y-pos, speed
					.paramUnits = { 'm','m', 'm', 'm', '%', ' ' },
					.paramDefaultVals = { 0, 0, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboFunction_Wait,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 1,
					.name = { 'W','a','i','t',' ',' ',' ',' ', 0 },
					.paramNames =  { 'T', 0, 0 }, // delay
					.paramUnits = { 'm','s',0,0, 0, 0 },
					.paramDefaultVals = { 1000, 0, 0 },
					.paramIncrement = { 100, 0, 0 }
		},
		(RoboFunDef) {
			.function = RoboFunction_ResetSensors,
					.steering = MovementAlgorithmType_RotateAndGo,
					.paramCount = 0,
					.name = { 'R','s','t','S','e','n','s',' ', 0 },
					.paramNames =  { 0, 0, 0 }, // delay
					.paramUnits = { 0,0,0,0, 0, 0 },
					.paramDefaultVals = { 0, 0, 0 },
					.paramIncrement = { 0, 0, 0 }
		}
};

const RoboFunDef* GetRoboFunctionDefinition(enum RoboFunctionType funId)
{
	uint8_t funNum = (funId & 0xFF);
	if(funNum >= RoboFunction_FunctionsCount)
		return 0;

	return &(_roboFuns[funNum]);
}
