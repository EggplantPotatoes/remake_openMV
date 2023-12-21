#ifndef __ST7735_H__
#define __ST7735_H__

#include "main.h"
#include <stdbool.h>
//#define ST7735_RST_Pin SPI1_RST_Pin
//#define ST7735_RST_GPIO_Port SPI1_RST_GPIO_Port
//#define ST7735_DC_Pin SPI1_DC_Pin
//#define ST7735_DC_GPIO_Port SPI1_DC_GPIO_Port
//#define ST7735_CS_Pin SPI1_CS_Pin
//#define ST7735_CS_GPIO_Port SPI1_CS_GPIO_Port
//
//#define ST7735_SPI_INSTANCE hspi1

#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128

#define ST7735_ROTATION 3

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))


void ST7735_Init(void);
void ST7735_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) ;
void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void ST7735_WriteData(uint8_t data);
#endif // __ST7735_H__
