/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "stm32l0xx.h"
#include "util.hpp"
#include "gpio.hpp"
#include "systick.hpp"

// The current clock frequency
uint32_t SystemCoreClock = 2097000;
volatile uint32_t ticker = 0;

struct PC13 : public Pin<13, 2>
{
    static constexpr uint32_t base{GPIOC_BASE};
};

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    Os os;

    Gpio<PC13> led(PinMode::output);

    Systick t;
    //t.start(SystemCoreClock / 1000); // 1000 ticks per second
    t.start(2097); // 1000 ticks per second

    // startup indication
    for (int i = 0; i < 6; i++)
    {
        led.on();
        os.delay(2);
        led.off();
        os.delay(150);
    }

    while (1)
    {
        led.on();
        os.delay(5);

        led.off();
        os.delay(1000);
    }
}
