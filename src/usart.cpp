#include "board.hpp"

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