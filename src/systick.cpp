#include "stm32l0xx.h"
#include "systick.hpp"

// The current clock frequency
uint32_t SystemCoreClock = 2097000;

// private static class member
unsigned volatile Systick::count = 0;
volatile Systick::CTRL_REGISTER &Systick::ctrl = *reinterpret_cast<CTRL_REGISTER *>(CTRL);

void Systick::init(unsigned systemCoreClock, unsigned ticks)
{
    regwrite<LOAD>(systemCoreClock / ticks - 1);
    regwrite<VAL>(0);
    //ctrl.bit.CLKSOURCE = 1;
    //ctrl.bit.TICKINT = 1;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    enable();
}

void Systick::enable()
{
    //ctrl.bit.ENABLE = 1;
}

void Systick::disable()
{
    //ctrl.bit.ENABLE = 0;
}

void Systick::tick()
{
    count++;
}

void Systick::delay(unsigned ticks)
{
    unsigned start = count;
    while (count - start < ticks)
        ;
}

// Interrupt handler, prevent name mangling
extern "C" void SysTick_Handler(void)
{
    Systick::tick();
}