#pragma once

struct Rcc
{
    RCC_TypeDef &rcc = *reinterpret_cast<RCC_TypeDef *>(RCC_BASE);
    void gpioClockEnable(uint32_t mask) { rcc.IOPENR |= mask; }
};
