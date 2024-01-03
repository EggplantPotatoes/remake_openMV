/*
 * camera_run.h
 *
 *  Created on: Nov 17, 2023
 *      Author: smile
 */

#ifndef CAMERA_RUN_H_
#define CAMERA_RUN_H_

#include "main.h"

#define RGB565_OUTPUT_HEIGHT   160
#define RGB565_OUTPUT_WIDTH    128

#define JPEG_OUTPUT_WIDTH  128
#define JPEG_OUTPUT_HEIGHT 160

extern uint16_t framebuffer[RGB565_OUTPUT_HEIGHT*RGB565_OUTPUT_WIDTH] __attribute__((section(".sram_data2")));
typedef struct
{
    uint8_t frame_sta;
}camera_t;

extern camera_t cam;

void camera_RGB565_init(void);
void TFT_LCD_RGB565_run(void);
void camera_JPEG_init(void);
void JPEG_run(void);

#endif /* CAMERA_RUN_H_ */
