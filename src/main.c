/**
 * STM32L071 skeleton
 *
 * John Berg @ netbasenext.nl
 */

#include "board.hpp"

/**
 * global objects and variable initialization
 */
Systick systick;
uint32_t SystemCoreClock = 2097000;

// Blocking send
int USART1_send(const char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        while (!(USART1->ISR & USART_ISR_TXE))
            ;
        USART1->TDR = *ptr++;
    }
    return len;
}

const char *msg = "Hello!\r\n";

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    systick.start(SystemCoreClock / 1000); // 1000 ticks per second

    //Enable the clock to PORT C
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    //Step 2: Configure PC13 as output, push pull
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // clear bits 26 and 27 to zero
    GPIOC->MODER |= GPIO_MODER_MODE13_0;

    //Enable the clock to PORT A
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    // USART 1
    // Use system clock for USART1
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
    MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART1SEL, RCC_CCIPR_USART1SEL_0);

    // PA9 (TxD) shall use the alternate function 4 (see data sheet)
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL9, 4UL << GPIO_AFRH_AFSEL9_Pos);
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE9, GPIO_MODER_MODE9_1);

    // PA10 (RxD) shall use the alternate function 4 (see data sheet)
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL10, 4UL << GPIO_AFRH_AFSEL10_Pos);
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE10, GPIO_MODER_MODE10_1);

    // Set baudrate
    USART1->BRR = (SystemCoreClock / 9600);

    // Enable transmitter, receiver and receive-interrupt of USART1
    USART1->CR1 = USART_CR1_UE + USART_CR1_TE + USART_CR1_RE + USART_CR1_RXNEIE;

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(USART1_IRQn);

    unsigned cnt = 0;

    while (1)
    {
        //Step 3: Set PC13 high
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
        systick.delay(500);

        //Step 4: Reset PC13 low
        GPIOC->BSRR |= GPIO_BSRR_BR_13;
        systick.delay(500);

        USART1_send(msg, 8);
    }

    return 0;
}
