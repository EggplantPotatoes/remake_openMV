/*
 * lcd_display.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Do
 */

#include "lcd_display.h"
#include "st7735.h"
#include "fonts.h"



void lcd_display_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *image)
{
	ST7735_DrawImage(x,y,width,height,image);

}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//得到字体一个字符对应点阵集所占的字节数
    num = num - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    for (t = 0; t < csize; t++)
    {
        if (size == 12)temp = asc2_1206[num][t]; 	 	//调用1206字体
        else if (size == 16)temp = asc2_1608[num][t];	//调用1608字体
        else if (size == 24)temp = asc2_2412[num][t];	//调用2412字体
        else if (size == 32)temp = asc2_3216[num][t];	//调用3216字体
        else return;								//没有的字库

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)ST7735_DrawPixel(x, y, RED);
            else if (mode == 0)ST7735_DrawPixel(x, y, WHITE);

            temp <<= 1;
            y++;

            if (y >= 600)return;		//超区域了

            if ((y - y0) == size)
            {
                y = y0;
                x++;

                if (x >= 1024)return;	//超区域了

                break;
            }
        }
    }
}
//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0);
                continue;
            }
            else enshow = 1;

        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0);
    }
}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80)LCD_ShowChar(x + (size / 2)*t, y, '0', size, mode & 0X01);
                else LCD_ShowChar(x + (size / 2)*t, y, ' ', size, mode & 0X01);

                continue;
            }
            else enshow = 1;

        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, mode & 0X01);
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break; //退出

        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }
}

void lcd_init(void)
{
	uint32_t i;
	HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
	ST7735_Init();
	ST7735_FillScreen(RED);
	HAL_Delay(200);
	ST7735_FillScreen(GREEN);
	HAL_Delay(200);
	ST7735_FillScreen(BLUE);
	HAL_Delay(200);
	ST7735_FillScreen(WHITE);
	HAL_Delay(200);
//	LCD_ShowString(0, 0, 160, 128, 16, " lcd display test ");
//    for(i=0;i<100;i++)
//    {
//    	ST7735_DrawPixel(i,i,RED);
//    }
}

