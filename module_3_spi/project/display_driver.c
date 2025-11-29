#include "display_driver.h"
#include "spi_driver.h"

/* Commands for a hypothetical Display Controller */
#define CMD_SLEEP_OUT   0x11
#define CMD_DISPLAY_ON  0x29
#define CMD_SET_CURSOR  0x2A
#define CMD_WRITE_RAM   0x2C

/* 
 * Helper: Chip Select Control 
 * In a real system, this would toggle a GPIO pin.
 */
void CS_Low(void) { /* GPIO->ODR &= ~PIN_CS; */ }
void CS_High(void) { /* GPIO->ODR |= PIN_CS; */ }

void Display_SendCommand(uint8_t cmd)
{
    CS_Low();
    /* In real display, we might need to toggle a Data/Command (DC) pin here */
    SPI_Transfer(cmd);
    CS_High();
}

void Display_SendData(uint8_t data)
{
    CS_Low();
    SPI_Transfer(data);
    CS_High();
}

void Display_Init(void)
{
    SPI_Init();
    
    /* Wake up display */
    Display_SendCommand(CMD_SLEEP_OUT);
    /* Wait 120ms... */
    
    /* Turn on display */
    Display_SendCommand(CMD_DISPLAY_ON);
}

void Display_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    /* Set Cursor Position */
    Display_SendCommand(CMD_SET_CURSOR);
    Display_SendData(x >> 8);
    Display_SendData(x & 0xFF);
    Display_SendData(y >> 8);
    Display_SendData(y & 0xFF);
    
    /* Write Pixel Data */
    Display_SendCommand(CMD_WRITE_RAM);
    Display_SendData(color >> 8);
    Display_SendData(color & 0xFF);
}

void Display_FillScreen(uint16_t color)
{
    /* Optimized: Set window to full screen and blast pixels */
    /* For simulation, we just loop pixels */
    for (int y = 0; y < 240; y++)
    {
        for (int x = 0; x < 320; x++)
        {
            Display_DrawPixel(x, y, color);
        }
    }
}

void Display_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            Display_DrawPixel(x + j, y + i, color);
        }
    }
}
