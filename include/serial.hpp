#pragma once

#include "stm32l0xx.h"
#include <type_traits>

#include "gpio.hpp"

template <typename Tx, typename Rx>
struct Serial
{
    static_assert(std::is_base_of<Pin, Tx>::value, "Tx must derive from Pin");
    static_assert(std::is_base_of<Pin, Rx>::value, "Rx must derive from Pin");

    Serial()
    {
        Rcc rcc;
        rcc.gpioClockEnable(Tx::portmask);
        rcc.uartClockEnable(RCC_APB2ENR_USART1EN);

        Gpio<Tx> tx(PinMode::alternate, 4);
        Gpio<Tx> rx(PinMode::alternate, 4);
    }

    USART_TypeDef &uart = *reinterpret_cast<USART_TypeDef *>(USART1_BASE);

    void send(const char *ptr)
    {
        while (*ptr != 0)
        {
            while (!(uart.ISR & USART_ISR_TXE))
                ;
            uart.TDR = *ptr++;
        }
    }

    void baudrate(unsigned rate)
    {
        uart.BRR = (SystemCoreClock / rate);
    }
};