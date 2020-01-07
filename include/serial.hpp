#pragma once

#include "board.hpp"

class Serial
{
public:
    Serial(PIN rx, PIN tx)
    {
    }

    void send(const char *ptr);
};