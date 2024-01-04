/*
 * lcd_display.h
 *
 *  Created on: Dec 9, 2023
 *      Author: Do
 */

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "main.h"
#include "st7735.h"



void lcd_init(void);
void lcd_display_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *image);
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p);
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
#endif /* LCD_DISPLAY_H_ */
