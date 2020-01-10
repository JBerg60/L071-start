#pragma once

#include <type_traits>

template <typename Enumeration>
auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

extern volatile uint32_t ticker;

struct Os
{
    void delay(uint32_t ticks)
    {
        unsigned start = ticker;
        while (ticker - start < ticks)
            ;
    }
};