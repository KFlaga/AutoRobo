#include "Initialization.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

void Initialize()
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |
			RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;

	// Set systimer tick to 1ms
	// It's main timer, on every tick will execute
	// all registered functions and update registered
	// counters
	if( SysTick_Config(SystemCoreClock/6000) ) // 1ms
	{

	}
}
