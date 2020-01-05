/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "board.hpp"
#include "serial.hpp"

/**
 * global objects and variable initialization
 */
Systick systick;
uint32_t SystemCoreClock = 2097000;

Serial bt(PA9, PA10);

extern void gpio_init();
extern void uart_init();
extern void USART1_send(const char *ptr);

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    systick.start(SystemCoreClock / 1000); // 1000 ticks per second

    gpio_init();
    uart_init();

    unsigned cnt = 0;

    while (1)
    {
        //Step 3: Set PC13 high
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
        systick.delay(500);

        //Step 4: Reset PC13 low
        GPIOC->BSRR |= GPIO_BSRR_BR_13;
        systick.delay(500);

        bt.send("Hello!\r\n");
    }

    return 0;
}
