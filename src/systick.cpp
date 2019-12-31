#include <stdint.h>

// The current clock frequency
uint32_t SystemCoreClock = 2097000;

// Counts milliseconds
volatile uint32_t systick_count = 0;

// Interrupt handler
void SysTick_Handler(void)
{
    systick_count++;
}