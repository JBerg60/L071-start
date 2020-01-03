#pragma once

#include "stm32l0xx.h"
#include "register.hpp"

class Systick : public Register
{
private:
    volatile unsigned count = 0;

    union CTRL_REGISTER {
        volatile uint32_t reg;

        struct
        {
            unsigned ENABLE : 1;
            unsigned TICKINT : 1;
            unsigned CLKSOURCE : 1;
        } bit;
    };

    SysTick_Type &regs = *reinterpret_cast<SysTick_Type *>(SysTick_BASE);

public:
    void start(uint32_t ticks);
    void stop();
    void tick();
    void delay(uint32_t ticks);
};