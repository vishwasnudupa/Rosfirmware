/*
 * [FIRMWARE LAYER: DRIVER / HAL]
 * 
 * This code interacts directly with the Hardware Registers (Silicon).
 * It abstracts the "Physics" (Voltage levels, Timing) into "Functions".
 */

#include "uart_driver.h"

#include "uart_driver.h"

void UART_Init(void)
{
#ifdef TARGET_QEMU_MPS2
    /* CMSDK UART Initialization */
    UART1->BAUDDIV = 16000000 / 115200;
    UART1->CTRL = UART_CTRL_TXEN | UART_CTRL_RXEN;
#else
    /* STM32 Initialization */
    UART1->BRR = 139;
    UART1->CR1 = UART_CR1_TE | UART_CR1_RE | UART_CR1_UE;
#endif
}

void UART_WriteChar(char c)
{
#ifdef TARGET_QEMU_MPS2
    /* Wait if TX Buffer is Full */
    while (UART1->STATE & UART_STATE_TXFULL);
    UART1->DATA = c;
#else
    while (!(UART1->SR & UART_SR_TXE));
    UART1->DR = c;
#endif
}

void UART_WriteString(const char *str)
{
    while (*str)
    {
        UART_WriteChar(*str++);
    }
}

char UART_ReadChar(void)
{
#ifdef TARGET_QEMU_MPS2
    /* Wait if RX Buffer is Empty */
    while (UART1->STATE & UART_STATE_RXEMPTY);
    return (char)(UART1->DATA);
#else
    while (!(UART1->SR & UART_SR_RXNE));
    return (char)(UART1->DR & 0xFF);
#endif
}
