#include "stm32l0xx.h"
#include "systick.hpp"

// The current clock frequency
uint32_t SystemCoreClock = 2097000;

// Counts milliseconds
volatile uint32_t systick_count = 0;

void Systick::init(uint32_t systemCoreClock, uint32_t ticks)
{
    SysTick->LOAD = systemCoreClock / ticks - 1;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void Systick::tick()
{
    systick_count++;
}

void Systick::delay_ms(uint32_t ms)
{
    uint32_t start = systick_count;
    while (systick_count - start < ms)
        ;
}

// Interrupt handler, prevent name mangling
extern "C" void SysTick_Handler(void)
{
    Systick::tick();
}