/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "stm32l0xx.h"
#include "util.hpp"
#include "gpio.hpp"
#include "systick.hpp"
#include "serial.hpp"

// The current clock frequency
uint32_t SystemCoreClock = 2097000;
volatile uint32_t ticker = 0;

struct PA9 : public Pindef<9, 0>
{
    static constexpr uint32_t base{GPIOA_BASE};
    enum ALTFUNC : unsigned
    {
        MCO,
        UART1_TX = 4,
        I2C_SCL = 6,
        I2C3_SMBA
    };
};

struct PA10 : public Pindef<10, 0>
{
    static constexpr uint32_t base{GPIOA_BASE};
    enum ALTFUNC : unsigned
    {
        MCO,
        UART1_RX = 4,
        I2C_SDA = 6
    };
};

struct PC13 : public Pindef<13, 2>
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
    t.start(SystemCoreClock / 1000); // 1000 ticks per second

    Serial<PA9, PA10> bt;

    // Use system clock for USART1
    MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART1SEL, RCC_CCIPR_USART1SEL_0);

    bt.baudrate(9600);

    // Enable transmitter, receiver and receive-interrupt of USART1
    USART1->CR1 = USART_CR1_UE + USART_CR1_TE + USART_CR1_RE + USART_CR1_RXNEIE;

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(USART1_IRQn);

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

        bt.send("Hello!\r\n");
    }
}
