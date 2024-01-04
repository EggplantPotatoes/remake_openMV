/*
 * ov5640_cfg.h
 *
 *  Created on: Nov 17, 2023
 *      Author: smile
 */

#ifndef OV5640_CFG_H_
#define OV5640_CFG_H_

#include"main.h"

#define OV5640_FW_DOWNLOAD_ADDR 0x8000

typedef struct
{
    uint16_t reg;
    uint8_t dat;
} ov5640_reg_cfg_t;

//extern ov5640_reg_cfg_t ov5640_init_cfg[207];
extern ov5640_reg_cfg_t ov5640_init_cfg[300];
//extern ov5640_reg_cfg_t ov5640_init_cfg[272];
extern ov5640_reg_cfg_t ov5640_rgb565_cfg[45];
//extern ov5640_reg_cfg_t ov5640_rgb565_cfg[42];

extern ov5640_reg_cfg_t ov5640_jpeg_cfg[41];
extern uint8_t ov5640_auto_focus_firmware[4077];

#endif
