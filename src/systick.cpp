#include "systick.hpp"

extern volatile uint32_t ticker;

void Systick::start(uint32_t ticks)
{
    regs.LOAD = ticks - 1;
    regs.VAL = 0;
    regs.CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

// Interrupt handler, prevent name mangling
extern "C" void SysTick_Handler(void)
{
    ticker++;
}
