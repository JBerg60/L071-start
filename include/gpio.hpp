#pragma once

#include "util.hpp"
#include "rcc.hpp"

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
