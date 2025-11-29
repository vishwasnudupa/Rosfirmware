/*
 * [FIRMWARE LAYER: APPLICATION]
 * 
 * This is the "Business Logic". 
 * It uses the Drivers to perform useful work (CLI, Dashboard).
 * Ideally, this code should not touch any registers directly.
 */

#include "uart_driver.h"
#include <stdint.h>
#include <string.h>

/* Ring Buffer Implementation */
#define BUFFER_SIZE 128
volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

/* Interrupt Service Routine */
void UART1_IRQHandler(void)
{
    /* Check if RXNE (Read Data Register Not Empty) is set */
    if (UART1->SR & UART_SR_RXNE)
    {
        /* Read data (clears RXNE) */
        char c = (char)(UART1->DR & 0xFF);
        
        /* Store in Ring Buffer */
        uint8_t next_head = (rx_head + 1) % BUFFER_SIZE;
        if (next_head != rx_tail) /* Check for overflow */
        {
            rx_buffer[rx_head] = c;
            rx_head = next_head;
        }
    }
}

/* Function to read from Ring Buffer */
int UART_Read(char *c)
{
    if (rx_head == rx_tail)
    {
        return 0; /* Buffer Empty */
    }
    
    *c = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % BUFFER_SIZE;
    return 1; /* Success */
}

/* Simple Command Line Interface */
void Process_Command(char *cmd)
{
    if (strcmp(cmd, "help") == 0)
    {
        UART_WriteString("Available commands: help, led_on, led_off, status\r\n");
    }
    else if (strcmp(cmd, "led_on") == 0)
    {
        UART_WriteString("LED turned ON (Virtual)\r\n");
    }
    else if (strcmp(cmd, "led_off") == 0)
    {
        UART_WriteString("LED turned OFF (Virtual)\r\n");
    }
    else
    {
        UART_WriteString("Unknown command. Type 'help'.\r\n");
    }
}

int main(void)
{
    char cmd_buffer[32];
    uint8_t cmd_idx = 0;
    char c;

    UART_Init();
    
    /* Enable RX Interrupt */
    UART1->CR1 |= UART_CR1_RXNEIE;
    
    /* Enable Interrupt in NVIC (Simulated: In real hardware, NVIC_EnableIRQ(USART1_IRQn)) */
    
    UART_WriteString("\r\n=== Virtual Instrument Cluster CLI ===\r\n");
    UART_WriteString("> ");

    while (1)
    {
        /* Check if we have data in the ring buffer */
        if (UART_Read(&c))
        {
            /* Echo back */
            UART_WriteChar(c);
            
            /* Handle Enter Key */
            if (c == '\r' || c == '\n')
            {
                UART_WriteString("\r\n");
                cmd_buffer[cmd_idx] = '\0';
                Process_Command(cmd_buffer);
                cmd_idx = 0;
                UART_WriteString("> ");
            }
            else if (cmd_idx < 31)
            {
                cmd_buffer[cmd_idx++] = c;
            }
        }
    }
    
    return 0;
}
