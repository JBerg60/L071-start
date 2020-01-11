/**
 * STM32L071 Os and utility functions
 *
 * John Berg @ netbasenext.nl
 */

#include "stm32l0xx.h"

#include "os.hpp"

extern volatile uint32_t ticker;

void Os::delay(uint32_t ticks)
{
    unsigned start = ticker;
    while (ticker - start < ticks)
        ;
}

// Change system clock to 32 MHz using 8 MHz crystal
extern "C" void SystemInit()
{
    // Because the debugger switches PLL on, we may
    // need to switch back to the HSI oscillator without PLL

    // Switch to HSI oscillator
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);

    // Wait until the switch is done
    //while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI)
    //{
    //}

    // Disable the PLL, then we can configure it
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

    // Flash latency 1 wait state
    SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY);

    // Enable HSE oscillator
    SET_BIT(RCC->CR, RCC_CR_HSEON);

    // Wait until HSE oscillator is ready
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY))
    {
    }

    // 32 MHz using the 8 MHz HSE oscillator multiply by 8 divide by 2
    WRITE_REG(RCC->CFGR, RCC_CFGR_PLLSRC_HSE + RCC_CFGR_PLLMUL8 + RCC_CFGR_PLLDIV2);

    // Enable PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);

    // Wait until PLL is ready
    while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
    {
    }

    // Select PLL as clock source
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    // Update variable
    SystemCoreClock = 32000000;

    // Switch the MSI oscillator off
    CLEAR_BIT(RCC->CR, RCC_CR_MSION);
}
