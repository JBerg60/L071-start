#include "register.hpp"

class Systick : public Register
{
private:
    static volatile unsigned count;

    enum : uint32_t
    {
        BASE = 0xE000E010,  /*!< (@ 0xE000E010) STK Structure                                              */
        CTRL = BASE,        /*!< (@ 0x00000000) SysTick control and status register                        */
        LOAD = BASE + 0x04, /*!< (@ 0x00000004) SysTick reload value register                              */
        VAL = BASE + 0x08,  /*!< (@ 0x00000008) SysTick current value register                             */
        CALIB = BASE + 0x0C /*!< (@ 0x0000000C) SysTick calibration value register                         */
    };

    union CTRL_REGISTER {
        volatile uint32_t reg;

        struct
        {
            unsigned ENABLE : 1;
            unsigned TICKINT : 1;
            unsigned CLKSOURCE : 1;
        } bit;
    };

    static volatile CTRL_REGISTER &ctrl;

public:
    static void init(unsigned systemCoreClock, unsigned ticks);
    static void tick();
    static void delay(unsigned ticks);
    static void enable();
    static void disable();
};