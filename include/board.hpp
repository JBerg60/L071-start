#pragma once

#include "stm32l0xx.h"
#include "systick.hpp"

// The global systick timer
extern Systick systick;

#define EEMEM __attribute__((section(".eeprom")))
extern EEMEM uint8_t eepromArray[4095];

enum PIN
{
    PA0 = 0,
    PA9 = 9,
    PA10
};