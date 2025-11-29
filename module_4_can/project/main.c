#include "can_driver.h"
#include "uart_driver.h"
#include <stdio.h>

/* Message IDs */
#define CAN_ID_RPM      0x100
#define CAN_ID_SPEED    0x101

void delay(volatile uint32_t count)
{
    while (count--) __asm("nop");
}

int main(void)
{
    CAN_Frame tx_frame;
    CAN_Frame rx_frame;
    uint16_t rpm = 0;
    uint8_t speed = 0;
    char log_buf[64];

    /* Initialize UART for Debugging */
    UART_Init();
    UART_WriteString("CAN Bus Simulator Started...\r\n");

    CAN_Init();

    while (1)
    {
        /* 1. Simulate Engine: Increase RPM */
        rpm += 50;
        if (rpm > 8000) rpm = 800;
        
        speed = rpm / 100;

        /* 2. Prepare RPM Message */
        tx_frame.ID = CAN_ID_RPM;
        tx_frame.DLC = 2;
        tx_frame.Data[0] = (rpm >> 8) & 0xFF;
        tx_frame.Data[1] = (rpm & 0xFF);

        /* 3. Transmit RPM */
        CAN_Transmit(&tx_frame);
        UART_WriteString("TX: RPM Frame Sent\r\n");

        /* 4. Check for incoming messages (Loopback) */
        if (CAN_Receive(&rx_frame))
        {
            /* In a real ECU, we would parse this. 
             * Here we just verify we received what we sent.
             */
            if (rx_frame.ID == CAN_ID_RPM)
            {
                UART_WriteString("RX: Loopback Received\r\n");
            }
        }

        delay(100000);
    }
    
    return 0;
}
