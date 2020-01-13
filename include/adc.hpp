#pragma once

#include "os.hpp"

struct Adc
{
    static void init()
    {
        // Enable clock for ADC
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADCEN);

        // Disable the ADC
        if (READ_BIT(ADC1->ISR, ADC_ISR_ADRDY))
        {
            SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);
        }
        if (READ_BIT(ADC1->CR, ADC_CR_ADEN))
        {
            SET_BIT(ADC1->CR, ADC_CR_ADDIS);
        }

        // Wait until ADC is disabled
        while (READ_BIT(ADC1->CR, ADC_CR_ADEN))
            ;

        // Enable ADC voltage regulator
        MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, 0);
        MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, ADC_CR_ADVREGEN);

        // Delay 1-2 ms
        Os os;
        os.delay(2);

        // Set low frequency mode (required if ADC clock is < 3,5MHz)
        SET_BIT(ADC->CCR, ADC_CCR_LFMEN);

        // ADC Clock = PCLK/2
        MODIFY_REG(ADC1->CFGR2, ADC_CFGR2_CKMODE, ADC_CFGR2_CKMODE_0);

        // Start calibration
        SET_BIT(ADC1->CR, ADC_CR_ADCAL);

        // Wait until the calibration is finished
        while (READ_BIT(ADC1->CR, ADC_CR_ADCAL))
            ;

        // Clear the ready flag
        SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);

        // Enable the ADC and wait until is ready
        SET_BIT(ADC1->CR, ADC_CR_ADEN);
        while (!READ_BIT(ADC1->ISR, ADC_ISR_ADRDY))
            ;

        // Select software start trigger
        MODIFY_REG(ADC1->CFGR1, ADC_CFGR1_EXTEN, 0);

        // Select single conversion mode
        CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);

        // Set sample time to 19.5 cycles
        MODIFY_REG(ADC1->SMPR, ADC_SMPR_SMP, ADC_SMPR_SMP_2);
    }

    static uint32_t read(unsigned channel)
    {
        // Select the channel
        WRITE_REG(ADC1->CHSELR, 1UL << channel);

        // Clear the finish flag
        CLEAR_BIT(ADC1->ISR, ADC_ISR_EOC);

        // Start a conversion
        SET_BIT(ADC1->CR, ADC_CR_ADSTART);

        // Wait until the conversion is finished
        while (!READ_BIT(ADC1->ISR, ADC_ISR_EOC))
            ;
        while (READ_BIT(ADC1->CR, ADC_CR_ADSTART))
            ;

        // Return the lower 12 bits of the result
        return ADC1->DR & 0b111111111111;
    }
};