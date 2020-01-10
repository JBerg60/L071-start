/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "stm32l0xx.h"
#include <type_traits>

// The current clock frequency
uint32_t SystemCoreClock = 2097000;

// delay loop for the default 2.1 MHz CPU clock with optimizer enabled
void delay(uint32_t msec)
{
    uint32_t j = 0;
    while (j++ < 419UL * msec)
    {
        __asm("nop");
    }
}

template <typename Enumeration>
auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

enum class PinMode : unsigned
{
    input = 0b00,
    output = 0b01,
    alternate = 0b10,
    analog = 0b11
};

enum class Ports
{

};

template <unsigned pinnr, unsigned portnr>
struct Pin
{
    static_assert(pinnr <= 15, "Pin number must be 0 .. 15!");

    static constexpr unsigned pin{pinnr};
    static constexpr unsigned mask{1 << pin};

    static constexpr unsigned pin2bit{pin * 2};
    static constexpr unsigned mask2bit{0b11 << pin2bit};

    static constexpr unsigned port{portnr};
    static constexpr unsigned portmask{1 << port};
};

struct PC13 : public Pin<13, 2>
{
    static constexpr uint32_t base{GPIOC_BASE};
};

struct Rcc
{
    RCC_TypeDef &rcc = *reinterpret_cast<RCC_TypeDef *>(RCC_BASE);
    void gpioClockEnable(uint32_t mask) { rcc.IOPENR |= mask; }
};

template <typename Pin>
struct Gpio
{
    Gpio(PinMode pinMode = PinMode::input)
    {
        Rcc rcc;
        rcc.gpioClockEnable(Pin::portmask);

        mode(pinMode);
    }

    GPIO_TypeDef &gpio = *reinterpret_cast<GPIO_TypeDef *>(Pin::base);

    void mode(PinMode mode)
    {
        gpio.MODER = (gpio.MODER & ~Pin::mask2bit) | as_integer(mode) << Pin::pin2bit;
    }

    void on() { gpio.BSRR = Pin::mask; }
    void off() { gpio.BSRR = Pin::mask << 16; }
    void toggle() { gpio.ODR ^= Pin::mask; }
    bool value() { return gpio.IDR & Pin::mask; }
};

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    Gpio<PC13> led(PinMode::output);

    // startup indication
    for (int i = 0; i < 8; i++)
    {
        led.on();
        delay(2);
        led.off();
        delay(80);
    }

    while (1)
    {
        led.on();
        delay(5);

        led.off();
        delay(1000);
    }
}