#include "ModulesControl.h"

#include "Timers.h"
#include "stm32f4xx_tim.h"
#include "Pins.h"

Module* _modules[6];
uint8_t _modulesCount = 0;
static bool _isRunning = FALSE;

void InitModuleControl()
{
	// Init modules timer on TIM3 CH1
	// Enable CC1 interrupt
	TIM3->DIER = 0x0002;
	// Set CC1 as output
	TIM3->CCER = 0x0001;
	// Tick on 100us : 84Mhz with /4 on APB2 = 21Mhz
	TIM3->PSC = 2099;
	TIM3->ARR = MODULE_TICK*10;
	// Interrupt on half of CNT
	TIM3->CCR1 = MODULE_TICK;
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler()
{
	if (TIM3 ->SR & TIM_SR_CC1IF )
	{
		uint16_t startCNT = TIM3->CNT;
		// Update each module
		uint8_t mod;
		uint16_t curCNT, diffCNT;
		for(mod = 0; mod < _modulesCount; mod++)
		{
			// Find how much time passed since last update
			curCNT = TIM3->CNT;
			if(_modules[mod]->lastUpdateCNT > curCNT)
			{
				// TIM2 overflowed
				diffCNT = curCNT + MODULE_TICK*10 - _modules[mod]->lastUpdateCNT;
			}
			else
				diffCNT = curCNT - _modules[mod]->lastUpdateCNT;
			// Update module
			_modules[mod]->lastUpdateCNT = curCNT;
			_modules[mod]->Update(diffCNT);
		}
		// Ensure at least 2ms break between interrupts
		curCNT = TIM3->CNT;
		if(curCNT < startCNT)
			diffCNT = curCNT + MODULE_TICK*10 - startCNT;
		else
			diffCNT = curCNT - startCNT;

		uint32_t reg = (uint16_t)( diffCNT < (uint16_t)MODULE_TICK - 20 ?
				curCNT + (uint16_t)MODULE_TICK - diffCNT : curCNT + 20);
		TIM3->CCR1 = reg;
	}
	TIM3->SR = 0;
}


void StartModuleUpdate()
{
	if(_isRunning)
		return;
	uint8_t mod;
	for(mod = 0; mod < _modulesCount; mod++)
	{
		_modules[mod]->lastUpdateCNT = 0;
		_modules[mod]->Start();
	}
	TIM3->CR1 |= 0x0001;
	_isRunning = TRUE;
}

void StopModuleUpdate()
{
	if(!_isRunning)
		return;
	uint8_t mod;
	for(mod = 0; mod < _modulesCount; mod++)
	{
		_modules[mod]->Stop();
	}
	TIM3->CR1 &= 0xFFFE;
	_isRunning = FALSE;
}

bool RegisterModule(Module* module)
{
	if(_isRunning || _modulesCount >= 6)
		return FALSE;

	_modules[_modulesCount] = module;
	_modulesCount++;
	return TRUE;
}

bool UnregisterModule(Module* module)
{
	if(_isRunning)
		return FALSE;

	uint8_t mod;
	for(mod = 0; mod < _modulesCount; mod++)
	{
		if(_modules[mod] == module)
		{
			_modulesCount--;
			for(;mod < _modulesCount; mod++)
				_modules[mod] = _modules[mod+1];
			return TRUE;
		}
	}
	return FALSE;
}

