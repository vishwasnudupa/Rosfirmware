#include "display_driver.h"

void delay(volatile uint32_t count)
{
    while (count--) __asm("nop");
}

int main(void)
{
    /* Initialize Display (which inits SPI) */
    Display_Init();
    
    /* Clear Screen to Black */
    Display_FillScreen(BLACK);
    
    /* Draw Dashboard Frame */
    /* Top Bar */
    Display_DrawRect(0, 0, 320, 20, BLUE);
    
    /* Speedometer Box */
    Display_DrawRect(50, 50, 100, 100, WHITE);
    
    /* RPM Box */
    Display_DrawRect(170, 50, 100, 100, RED);
    
    while (1)
    {
        /* Simulate updating a value bar */
        for (int i = 0; i < 80; i+=10)
        {
            Display_DrawRect(60, 140 - i, 20, 8, GREEN);
            delay(50000);
        }
        
        Display_DrawRect(60, 60, 20, 80, BLACK); /* Clear bar */
        delay(50000);
    }
    
    return 0;
}
