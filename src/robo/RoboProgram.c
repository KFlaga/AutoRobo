#include "RoboProgram.h"
#include "Timers.h"
#include "RoboModule.h"
#include "RoboMove.h"

const RoboFunDef _roboFuns[ROBOFUN_COUNT] __attribute__ ((section(".rodata"))) = {
		(RoboFunDef) {
			.function = 0,
					.paramCount = 0,
					.name = { 'N','U','L','L',' ',' ',' ',' ', 0 },
					.paramNames =  { ' ',' ',' ' },
					.paramUnits = { ' ',' ',' ',' ',' ',' ' },
					.paramDefaultVals = { 0,0,0 },
					.paramIncrement = { 0,0,0},
		},
		(RoboFunDef) {
			.function = RoboMoveFwd,
					.paramCount = 2,
					.name = { 'M','o','v','e','F','w','d',' ', 0 },
					.paramNames =  { 's','v',' ' }, // dist, vel,
					.paramUnits = { 'm','m','%',' ', 0, 0 },
					.paramDefaultVals = { 100, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboMoveBwd,
					.paramCount = 2,
					.name = { 'M','o','v','e','B','w','d',' ', 0 },
					.paramNames =  { 'D','V', 0 }, // dist, vel,
					.paramUnits = { 'm','m','%',' ', 0, 0 },
					.paramDefaultVals = { 100, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboRotateLeft,
					.paramCount = 2,
					.name = { 'R','o','t','L','e','f','t',' ', 0 },
					.paramNames =  { 'A', 'w', 0 }, // angle, speed
					.paramUnits = { 'd','g', '%', ' ', 0, 0 },
					.paramDefaultVals = { 90, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboRotateRight,
					.paramCount = 2,
					.name = { 'R','o','t','R','i','g','h','t', 0 },
					.paramNames =  { 'A', 'w', 0 }, // angle, speed
					.paramUnits = { 'd','g', '%', ' ', 0, 0 },
					.paramDefaultVals = { 90, 100, 0 },
					.paramIncrement = { 10, 10, 0 }
		},
		(RoboFunDef) {
			.function = RoboArcFwdLeft,
					.paramCount = 3,
					.name = { 'A','r','c','F','w','d','L',' ', 0 },
					.paramNames =  { 'R', 'A', 'w' }, // radius, angle, speed
					.paramUnits = { 'm','m', 'd', 'g', '%', ' ' },
					.paramDefaultVals = { 100, 90, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboArcFwdRight,
					.paramCount = 2,
					.name = { 'A','r','c','F','w','d','R',' ', 0 },
					.paramNames =  { 'R', 'A', 'w' }, // radius, angle, speed
					.paramUnits = { 'm','m', 'd', 'g', '%', ' ' },
					.paramDefaultVals = { 100, 90, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboArcBwdLeft,
					.paramCount = 2,
					.name = { 'A','r','c','B','w','d','L',' ', 0 },
					.paramNames =  { 'R', 'A', 'w' }, // radius, angle, speed
					.paramUnits = { 'm','m', 'd', 'g', '%', ' ' },
					.paramDefaultVals = { 100, 90, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboArcBwdRight,
					.paramCount = 2,
					.name = { 'A','r','c','B','w','d','R',' ', 0 },
					.paramNames =  { 'R', 'A', 'w' }, // radius, angle, speed
					.paramUnits = { 'm','m', 'd', 'g', '%', ' ' },
					.paramDefaultVals = { 100, 90, 100 },
					.paramIncrement = { 10, 10, 10 }
		},
		(RoboFunDef) {
			.function = RoboWait,
					.paramCount = 1,
					.name = { 'W','a','i','t',' ',' ',' ',' ', 0 },
					.paramNames =  { 'T', 0, 0 }, // delay
					.paramUnits = { 'm','s',0,0, 0, 0 },
					.paramDefaultVals = { 1000, 0, 0 },
					.paramIncrement = { 100, 0, 0 }
		}
};

__inline__ const RoboFunDef* GetRoboFunctionDefinition(uint8_t funId)
{
	if(funId >= ROBOFUN_COUNT)
		return 0;

	return &(_roboFuns[funId]);
}
