/*
 * ov5640.h
 *
 *  Created on: Dec 9, 2023
 *      Author: Do
 */

#ifndef OV5640_H_
#define OV5640_H_

#include "main.h"


/* OV5640模块芯片ID */
#define OV5640_CHIP_ID  0x5640


/* OV5640 SCCBͨѶ��ַ */
#define OV5640_IIC_ADDR                    0x78

/* OV5640模块固件下载超时时间，单位：毫秒（ms） */
#define OV5640_TIMEOUT  5000

/* OV5640ģ��ISP���봰�����ߴ� */
#define OV5640_ISP_INPUT_WIDTH_MAX          0x0A3F
#define OV5640_ISP_INPUT_HEIGHT_MAX         0x06A9



/* OV5640ģ��ƹ�ģʽö�� */
typedef enum
{
    OV5640_LIGHT_MODE_ADVANCED_AWB = 0x00,  /* Advanced AWB */
    OV5640_LIGHT_MODE_SIMPLE_AWB,           /* Simple AWB */
    OV5640_LIGHT_MODE_MANUAL_DAY,           /* Manual day */
    OV5640_LIGHT_MODE_MANUAL_A,             /* Manual A */
    OV5640_LIGHT_MODE_MANUAL_CWF,           /* Manual cwf */
    OV5640_LIGHT_MODE_MANUAL_CLOUDY         /* Manual cloudy */
} ov5640_light_mode_t;

/* OV5640ģ��ɫ�ʱ��Ͷ�ö�� */
typedef enum
{
    OV5640_COLOR_SATURATION_0 = 0x00,       /* +4 */
    OV5640_COLOR_SATURATION_1,              /* +3 */
    OV5640_COLOR_SATURATION_2,              /* +2 */
    OV5640_COLOR_SATURATION_3,              /* +1 */
    OV5640_COLOR_SATURATION_4,              /* 0 */
    OV5640_COLOR_SATURATION_5,              /* -1 */
    OV5640_COLOR_SATURATION_6,              /* -2 */
    OV5640_COLOR_SATURATION_7,              /* -3 */
    OV5640_COLOR_SATURATION_8,              /* -4 */
} ov5640_color_saturation_t;

/* OV5640ģ������ö�� */
typedef enum
{
    OV5640_BRIGHTNESS_0 = 0x00,             /* +4 */
    OV5640_BRIGHTNESS_1,                    /* +3 */
    OV5640_BRIGHTNESS_2,                    /* +2 */
    OV5640_BRIGHTNESS_3,                    /* +1 */
    OV5640_BRIGHTNESS_4,                    /* 0 */
    OV5640_BRIGHTNESS_5,                    /* -1 */
    OV5640_BRIGHTNESS_6,                    /* -2 */
    OV5640_BRIGHTNESS_7,                    /* -3 */
    OV5640_BRIGHTNESS_8,                    /* -4 */
} ov5640_brightness_t;

/* OV5640ģ��Աȶ�ö�� */
typedef enum
{
    OV5640_CONTRAST_0 = 0x00,               /* +4 */
    OV5640_CONTRAST_1,                      /* +3 */
    OV5640_CONTRAST_2,                      /* +2 */
    OV5640_CONTRAST_3,                      /* +1 */
    OV5640_CONTRAST_4,                      /* 0 */
    OV5640_CONTRAST_5,                      /* -1 */
    OV5640_CONTRAST_6,                      /* -2 */
    OV5640_CONTRAST_7,                      /* -3 */
    OV5640_CONTRAST_8,                      /* -4 */
} ov5640_contrast_t;

/* OV5640ģ��ɫ��ö�� */
typedef enum
{
    OV5640_HUE_0 = 0x00,                    /* -180 degree */
    OV5640_HUE_1,                           /* -150 degree */
    OV5640_HUE_2,                           /* -120 degree */
    OV5640_HUE_3,                           /* -90 degree */
    OV5640_HUE_4,                           /* -60 degree */
    OV5640_HUE_5,                           /* -30 degree */
    OV5640_HUE_6,                           /* 0 degree */
    OV5640_HUE_7,                           /* +30 degree */
    OV5640_HUE_8,                           /* +60 degree */
    OV5640_HUE_9,                           /* +90 degree */
    OV5640_HUE_10,                          /* +120 degree */
    OV5640_HUE_11,                          /* +150 degree */
} ov5640_hue_t;

/* OV5640ģ������Ч��ö�� */
typedef enum
{
	OV5640_SPECIAL_EFFECT_NORMAL = 0x00,    /* Normal */
    OV5640_SPECIAL_EFFECT_BW,               /* B&W */
    OV5640_SPECIAL_EFFECT_BLUISH,           /* Bluish */
    OV5640_SPECIAL_EFFECT_SEPIA,            /* Sepia */
    OV5640_SPECIAL_EFFECT_REDDISH,          /* Reddish */
    OV5640_SPECIAL_EFFECT_GREENISH,         /* Greenish */
    OV5640_SPECIAL_EFFECT_NEGATIVE,         /* Negative */
} ov5640_special_effect_t;

/* OV5640ģ���ع��ö�� */
typedef enum
{
    OV5640_EXPOSURE_LEVEL_0 = 0x00,         /* -1.7EV */
    OV5640_EXPOSURE_LEVEL_1,                /* -1.3EV */
    OV5640_EXPOSURE_LEVEL_2,                /* -1.0EV */
    OV5640_EXPOSURE_LEVEL_3,                /* -0.7EV */
    OV5640_EXPOSURE_LEVEL_4,                /* -0.3EV */
    OV5640_EXPOSURE_LEVEL_5,                /* default */
    OV5640_EXPOSURE_LEVEL_6,                /* 0.3EV */
    OV5640_EXPOSURE_LEVEL_7,                /* 0.7EV */
    OV5640_EXPOSURE_LEVEL_8,                /* 1.0EV */
    OV5640_EXPOSURE_LEVEL_9,                /* 1.3EV */
    OV5640_EXPOSURE_LEVEL_10,               /* 1.7EV */
} ov5640_exposure_level_t;

/* OV5640ģ�����ö�� */
typedef enum
{
    OV5640_SHARPNESS_OFF = 0x00,            /* Sharpness OFF */
    OV5640_SHARPNESS_1,                     /* Sharpness 1 */
    OV5640_SHARPNESS_2,                     /* Sharpness 2 */
    OV5640_SHARPNESS_3,                     /* Sharpness 3 */
    OV5640_SHARPNESS_4,                     /* Sharpness 4 */
    OV5640_SHARPNESS_5,                     /* Sharpness 5 */
    OV5640_SHARPNESS_6,                     /* Sharpness 6 */
    OV5640_SHARPNESS_7,                     /* Sharpness 7 */
    OV5640_SHARPNESS_8,                     /* Sharpness 8 */
    OV5640_SHARPNESS_AUTO,                  /* Sharpness Auto */
} ov5640_sharpness_t;

/* OV5640ģ�龵��/��תö�� */
typedef enum
{
    OV5640_MIRROR_FLIP_0 = 0x00,            /* MIRROR */
    OV5640_MIRROR_FLIP_1,                   /* FLIP */
    OV5640_MIRROR_FLIP_2,                   /* MIRROR & FLIP */
    OV5640_MIRROR_FLIP_3,                   /* Normal */
} ov5640_mirror_flip_t;

/* OV5640ģ�����ͼ��ö�� */
typedef enum
{
    OV5640_TEST_PATTERN_OFF = 0x00,         /* OFF */
    OV5640_TEST_PATTERN_COLOR_BAR,          /* Color bar */
    OV5640_TEST_PATTERN_COLOR_SQUARE,       /* Color square */
} ov5640_test_pattern_t;

/* OV5640���ͼ���ʽö�� */
typedef enum
{
    OV5640_OUTPUT_FORMAT_RGB565 = 0x00,     /* RGB565 */
    OV5640_OUTPUT_FORMAT_JPEG,              /* JPEG */
} ov5640_output_format_t;

/* OV5640��ȡ֡���ݷ�ʽö�� */
typedef enum
{
    OV5640_GET_TYPE_DTS_8B_NOINC = 0x00,    /* ͼ���������ֽڷ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    OV5640_GET_TYPE_DTS_8B_INC,             /* ͼ���������ֽڷ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
    OV5640_GET_TYPE_DTS_16B_NOINC,          /* ͼ�������԰��ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    OV5640_GET_TYPE_DTS_16B_INC,            /* ͼ�������԰��ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
    OV5640_GET_TYPE_DTS_32B_NOINC,          /* ͼ���������ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    OV5640_GET_TYPE_DTS_32B_INC,            /* ͼ���������ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
} ov5640_get_type_t;


/* 错误代码 */
#define OV5640_EOK      0   /* 没有错误 */
//#define OV5640_ERROR    1   /* 错误 */
#define OV5640_EINVAL   2   /* 非法参数 */
#define OV5640_ENOMEM   3   /* 内存不足 */
#define OV5640_EEMPTY   4   /* 资源为空 */
#define OV5640_ETIMEOUT 5   /* 操作超时 */
/**
  * @}
  */

/** @defgroup OV5640_Exported_Constants
  * @{
  */
#define OV5640_OK                      (0)
#define OV5640_ERROR                   (-1)
/**
  * @brief  OV5640 Features Parameters
  */
/* Camera resolutions */
#define OV5640_R160x120                 0x00U   /* QQVGA Resolution           */
#define OV5640_R320x240                 0x01U   /* QVGA Resolution            */
#define OV5640_R480x272                 0x02U   /* 480x272 Resolution         */
#define OV5640_R640x480                 0x03U   /* VGA Resolution             */
#define OV5640_R800x480                 0x04U   /* WVGA Resolution            */

/* Camera Pixel Format */
#define OV5640_RGB565                   0x00U   /* Pixel Format RGB565        */
#define OV5640_RGB888                   0x01U   /* Pixel Format RGB888        */
#define OV5640_YUV422                   0x02U   /* Pixel Format YUV422        */
#define OV5640_Y8                       0x07U   /* Pixel Format Y8            */
#define OV5640_JPEG                     0x08U   /* Compressed format JPEG          */

/* Polarity */
#define OV5640_POLARITY_PCLK_LOW  0x00U /* Signal Active Low          */
#define OV5640_POLARITY_PCLK_HIGH 0x01U /* Signal Active High         */
#define OV5640_POLARITY_HREF_LOW  0x00U /* Signal Active Low          */
#define OV5640_POLARITY_HREF_HIGH 0x01U /* Signal Active High         */
#define OV5640_POLARITY_VSYNC_LOW 0x01U /* Signal Active Low          */
#define OV5640_POLARITY_VSYNC_HIGH  0x00U /* Signal Active High         */

/* Mirror/Flip */
#define OV5640_MIRROR_FLIP_NONE         0x00U   /* Set camera normal mode     */
#define OV5640_FLIP                     0x01U   /* Set camera flip config     */
#define OV5640_MIRROR                   0x02U   /* Set camera mirror config   */
#define OV5640_MIRROR_FLIP              0x03U   /* Set camera mirror and flip */

/* Zoom */
#define OV5640_ZOOM_x8                  0x00U   /* Set zoom to x8             */
#define OV5640_ZOOM_x4                  0x11U   /* Set zoom to x4             */
#define OV5640_ZOOM_x2                  0x22U   /* Set zoom to x2             */
#define OV5640_ZOOM_x1                  0x44U   /* Set zoom to x1             */

/* Special Effect */
#define OV5640_COLOR_EFFECT_NONE        0x00U   /* No effect                  */
#define OV5640_COLOR_EFFECT_BLUE        0x01U   /* Blue effect                */
#define OV5640_COLOR_EFFECT_RED         0x02U   /* Red effect                 */
#define OV5640_COLOR_EFFECT_GREEN       0x04U   /* Green effect               */
#define OV5640_COLOR_EFFECT_BW          0x08U   /* Black and White effect     */
#define OV5640_COLOR_EFFECT_SEPIA       0x10U   /* Sepia effect               */
#define OV5640_COLOR_EFFECT_NEGATIVE    0x20U   /* Negative effect            */


/* Light Mode */
#define OV5640_LIGHT_AUTO               0x00U   /* Light Mode Auto            */
#define OV5640_LIGHT_SUNNY              0x01U   /* Light Mode Sunny           */
#define OV5640_LIGHT_OFFICE             0x02U   /* Light Mode Office          */
#define OV5640_LIGHT_HOME               0x04U   /* Light Mode Home            */
#define OV5640_LIGHT_CLOUDY             0x08U   /* Light Mode Claudy          */

/* Night Mode */
#define NIGHT_MODE_DISABLE              0x00U   /* Disable night mode         */
#define NIGHT_MODE_ENABLE               0x01U   /* Enable night mode          */

/* Colorbar Mode */
#define COLORBAR_MODE_DISABLE           0x00U   /* Disable colorbar mode      */
#define COLORBAR_MODE_ENABLE            0x01U   /* 8 bars W/Y/C/G/M/R/B/Bl    */
#define COLORBAR_MODE_GRADUALV          0x02U   /* Gradual vertical colorbar  */

/* Pixel Clock */
#define OV5640_PCLK_7M                  0x00U   /* Pixel Clock set to 7Mhz    */
#define OV5640_PCLK_8M                  0x01U   /* Pixel Clock set to 8Mhz    */
#define OV5640_PCLK_9M                  0x02U   /* Pixel Clock set to 9Mhz    */
#define OV5640_PCLK_12M                 0x04U   /* Pixel Clock set to 12Mhz   */
#define OV5640_PCLK_24M                 0x08U   /* Pixel Clock set to 24Mhz   */

/* 操作函数 */
uint8_t ov5640_init(void);                                                                              /* 初始化OV5640模块 */
uint8_t ov5640_auto_focus_init(void);                                                                   /* 初始化OV5640模块自动对焦 */
uint8_t ov5640_auto_focus_once(void);                                                                   /* OV5640模块自动对焦一次 */
uint8_t ov5640_auto_focus_continuance(void);                                                            /* OV5640模块持续自动对焦 */
void ov5640_led_on(void);                                                                               /* 开启OV5640模块闪光灯 */
void ov5640_led_off(void);                                                                              /* 关闭OV5640模块闪光灯 */
uint8_t ov5640_set_light_mode(ov5640_light_mode_t mode);                                            /* 设置OV5640模块灯光模式 */
uint8_t ov5640_set_color_saturation(ov5640_color_saturation_t saturation);                          /* 设置OV5640模块色彩饱和度 */
uint8_t ov5640_set_brightness(ov5640_brightness_t brightness);                                      /* 设置OV5640模块亮度 */
uint8_t ov5640_set_contrast(ov5640_contrast_t contrast);                                            /* 设置OV5640模块对比度 */
uint8_t ov5640_set_hue(ov5640_hue_t hue);                                                           /* 设置OV5640模块色相 */
uint8_t ov5640_set_special_effect(ov5640_special_effect_t effect);                                  /* 设置OV5640模块特殊效果 */
uint8_t ov5640_set_exposure_level(ov5640_exposure_level_t level);                                   /* 设置OV5640模块曝光度 */
uint8_t ov5640_set_sharpness_level(ov5640_sharpness_t sharpness);                                   /* 设置OV5640模块锐度 */
uint8_t ov5640_set_mirror_flip(ov5640_mirror_flip_t mirror_flip);                                   /* 设置OV5640模块镜像/翻转 */
uint8_t ov5640_set_test_pattern(ov5640_test_pattern_t pattern);                                     /* 设置OV5640模块测试图案 */
uint8_t ov5640_set_output_format(ov5640_output_format_t format);                                    /* 设置OV5640模块输出图像格式 */
uint8_t ov5640_set_isp_input_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);           /* 设置OV5640模块ISP输入窗口尺寸 */
uint8_t ov5640_set_pre_scaling_window(uint16_t x_offset, uint16_t y_offset);                            /* 设置OV5640模块预缩放窗口偏移 */
uint8_t ov5640_set_output_size(uint16_t width, uint16_t height);                                        /* 设置OV5640模块输出图像尺寸 */




#endif /* OV5640_H_ */
