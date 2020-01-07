#include "serial.hpp"

void uart_init()
{
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
}

// Blocking send
void Serial::send(const char *ptr)
{
    while (*ptr != 0)
    {
        while (!(USART1->ISR & USART_ISR_TXE))
            ;
        USART1->TDR = *ptr++;
    }
}

// Called after each received character
extern "C" void USART1_IRQHandler(void)
{
    // read the received character
    char received = USART1->RDR;

    // send echo back
    while (!(USART1->ISR & USART_ISR_TXE))
        ;
    USART1->TDR = received;
}