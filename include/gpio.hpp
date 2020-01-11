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

struct Pin
{
};

template <unsigned pinnr, unsigned portnr>
struct Pindef : public Pin
{
    static_assert(pinnr <= 15, "Pin number must be 0 .. 15!");

    static constexpr unsigned pin{pinnr};
    static constexpr unsigned mask{1 << pin};

    static constexpr unsigned pin2bit{pin * 2};
    static constexpr unsigned mask2bit{static_cast<unsigned>(0b11 << pin2bit)};

    static constexpr unsigned pin4bit{(pin % 8) * 4};
    static constexpr unsigned mask4bit{0b1111 << pin4bit};

    static constexpr unsigned afreg{(pin / 8) & 1};

    static constexpr unsigned port{portnr};
    static constexpr unsigned portmask{1 << port};
};

template <typename P>
struct Gpio
{
    static_assert(std::is_base_of<Pin, P>::value, "P must derive from Pin");

    Gpio(PinMode pinMode = PinMode::input)
    {
        Rcc rcc;
        rcc.gpioClockEnable(P::portmask);

        mode(pinMode);
    }

    Gpio(PinMode pinMode, unsigned altfunc)
    {
        Rcc rcc;
        rcc.gpioClockEnable(P::portmask);

        altpinfunc(altfunc);
        mode(pinMode);
    }

    GPIO_TypeDef &gpio = *reinterpret_cast<GPIO_TypeDef *>(P::base);

    void mode(PinMode mode)
    {
        gpio.MODER = (gpio.MODER & ~P::mask2bit) | as_integer(mode) << P::pin2bit;
    }

    void altpinfunc(unsigned altfunc)
    {
        gpio.AFR[P::afreg] = (gpio.AFR[P::afreg] & ~P::mask4bit) | altfunc << P::pin4bit;
    }

    void on() { gpio.BSRR = P::mask; }
    void off() { gpio.BSRR = P::mask << 16; }
    void toggle() { gpio.ODR ^= P::mask; }
    uint32_t value() { return gpio.IDR & P::mask; }

    void operator=(bool value) { value ? on() : off(); }
    operator uint32_t() { return value(); }
};
