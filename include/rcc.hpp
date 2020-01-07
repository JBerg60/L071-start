#pragma once

#include "board.hpp"

class Rcc
{
private:
    //static constexpr RCC_TypeDef *regs = reinterpret_cast<RCC_TypeDef *>(RCC_BASE);
    //static constexpr const void *ptr = &MY_PTR_SYMBOL;

public:
    static constexpr void GpioClockEnable(uint32_t mask)
    {
        //regs->IOPENR |= mask;
    }
};