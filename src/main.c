/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "board.hpp"

/**
 * global objects and variable initialization
 */
Systick systick;
uint32_t SystemCoreClock = 2097000;

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    systick.start(SystemCoreClock / 1000); // 1000 ticks per second

    //Step 1: Enable the clock to PORT C
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    //Step 2: Configure PC13 as output, push pull
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // clear bits 26 and 27 to zero
    GPIOC->MODER |= GPIO_MODER_MODE13_0;

    while (1)
    {
        //Step 3: Set PC13 high
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
        systick.delay(250);

        //Step 4: Reset PC13 low
        GPIOC->BSRR |= GPIO_BSRR_BR_13;
        systick.delay(250);
    }

    return 0;
}
