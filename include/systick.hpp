#pragma once

#include "stm32l0xx.h"

class Systick
{
private:
    SysTick_Type &regs = *reinterpret_cast<SysTick_Type *>(SysTick_BASE);

public:
    void start(uint32_t ticks);
};