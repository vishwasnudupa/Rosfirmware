#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <stdint.h>

/* Colors (16-bit RGB565) */
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F

/* Function Prototypes */
void Display_Init(void);
void Display_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void Display_FillScreen(uint16_t color);
void Display_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif
