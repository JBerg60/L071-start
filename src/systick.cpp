#include "board.hpp"

void Systick::start(uint32_t ticks)
{
    regs.LOAD = ticks - 1;
    regs.VAL = 0;

    regs.CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void Systick::stop()
{
    //ctrl.bit.ENABLE = 0;
}

void Systick::tick()
{
    count++;
}

void Systick::delay(uint32_t ticks)
{
    unsigned start = count;
    while (count - start < ticks)
        ;
}

// Interrupt handler, prevent name mangling
extern "C" void SysTick_Handler(void)
{
    systick.tick();
}