/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "stm32l0xx.h"
#include "system_stm32l0xx.h"

#include "systick.hpp"

#define EEMEM __attribute__((section(".eeprom")))
EEMEM uint8_t eepromArray[4095];

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    Systick::init(SystemCoreClock, 1000); // 1000 ticks per second

    //Step 1: Enable the clock to PORT C
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    //Step 2: Configure PC13 as output, push pull
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // clear bits 26 and 27 to zero
    GPIOC->MODER |= GPIO_MODER_MODE13_0;

    while (1)
    {
        //Step 3: Set PC13 high
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
        Systick::delay(250);

        //Step 4: Reset PC13 low
        GPIOC->BSRR |= GPIO_BSRR_BR_13;
        Systick::delay(250);
    }

    return 0;
}
