#include "rtos_kernel.h"
#include "uart_driver.h"
#include "display_driver.h"
#include "can_driver.h"

/* Shared Data */
volatile uint16_t current_rpm = 0;
volatile uint8_t current_speed = 0;

/* Task 1: Vehicle Data (CAN) */
void Task_CAN(void)
{
    CAN_Frame rx_frame;
    CAN_Init();
    
    while (1)
    {
        /* Check for new messages */
        if (CAN_Receive(&rx_frame))
        {
            if (rx_frame.ID == 0x100) /* RPM ID */
            {
                current_rpm = (rx_frame.Data[0] << 8) | rx_frame.Data[1];
                current_speed = current_rpm / 100;
            }
        }
        /* Yield to let other tasks run */
        /* RTOS_Delay(10); */ 
    }
}

/* Task 2: Dashboard UI (SPI) */
void Task_Display(void)
{
    Display_Init();
    Display_FillScreen(BLACK);
    
    /* Draw Static Elements */
    Display_DrawRect(0, 0, 320, 20, BLUE); /* Header */
    
    while (1)
    {
        /* Update RPM Bar */
        uint16_t bar_height = current_rpm / 100;
        Display_DrawRect(170, 150 - bar_height, 20, bar_height, RED);
        
        /* Update Speed Bar */
        Display_DrawRect(50, 150 - current_speed, 20, current_speed, WHITE);
        
        /* RTOS_Delay(50); */
    }
}

/* Task 3: Diagnostics (UART) */
void Task_Console(void)
{
    UART_Init();
    UART_WriteString("System Booted.\r\n");
    
    while (1)
    {
        /* Send Telemetry */
        UART_WriteString("RPM: ");
        /* (Simplified integer to string would go here) */
        UART_WriteString("...\r\n");
        
        /* RTOS_Delay(500); */
    }
}

int main(void)
{
    RTOS_Init();
    
    RTOS_CreateTask(Task_CAN, 1);
    RTOS_CreateTask(Task_Display, 2);
    RTOS_CreateTask(Task_Console, 3);
    
    RTOS_Start(); /* Never returns */
    
    return 0;
}
