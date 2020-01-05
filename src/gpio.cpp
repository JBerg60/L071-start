#include "board.hpp"

void gpio_init()
{
    //Enable the clock to PORT C
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    //Step 2: Configure PC13 as output, push pull
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // clear bits 26 and 27 to zero
    GPIOC->MODER |= GPIO_MODER_MODE13_0;
}