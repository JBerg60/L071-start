#include <stdint.h>

struct Systick
{
    static void init(uint32_t systemCoreClock, uint32_t ticks);
    static void tick();
    static void delay_ms(uint32_t ms);
};