/*
 * ov5640.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Do
 */

#include "ov5640.h"
#include "ov5640_cfg.h"
#include "ov5640_reg.h"
#include "IIC_bus.h"
#include "usb_bsp.h"

//uint8_t test_read = 0;

/* OV5640寄存器块枚举 */
typedef enum
{
    OV5640_REG_BANK_DSP = 0x00, /* DSP寄存器块 */
    OV5640_REG_BANK_SENSOR,     /* Sensor寄存器块 */
} ov5640_reg_bank_t;

/* OV5640模块数据结构体 */
static struct
{
    struct {
        uint16_t width;
        uint16_t height;
    } isp_input;
    struct {
        uint16_t width;
        uint16_t height;
    } pre_scaling;
    struct {
        uint16_t width;
        uint16_t height;
    } output;
} ov5640_sta = {0};

/**
 * @brief       OV5640模块写寄存器
 * @param       reg: 寄存器地址
 *              dat: 待写入的值
 * @retval      无
 */
static void ov5640_write_reg(uint16_t reg, uint8_t dat)
{
    BSP_I2C2_WriteReg16(OV5640_IIC_ADDR,reg,&dat,1);
}

/**
 * @brief       OV5640模块读寄存器
 * @param       reg: 寄存器的地址
 * @retval      读取到的寄存器值
 */
static uint8_t ov5640_read_reg(uint16_t reg)
{
    uint8_t dat = 0;

    BSP_I2C2_ReadReg16(OV5640_IIC_ADDR,reg,&dat,1);

    return dat;
}

/**
 * @brief       OV5640模块硬件初始化
 * @param       无
 * @retval      无
 */
static void ov5640_hw_init(void)
{
    HAL_GPIO_WritePin(DCMI_RST_GPIO_Port, DCMI_RST_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_SET);

}

/**
 * @brief       OV5640模块退出掉电模式
 * @param       无
 * @retval      无
 */
static void ov5640_exit_power_down(void)
{
	HAL_GPIO_WritePin(DCMI_RST_GPIO_Port, DCMI_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(DCMI_RST_GPIO_Port, DCMI_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(20);
}

/**
 * @brief       OV5640模块硬件复位
 * @param       无
 * @retval      无
 */
static void ov5640_hw_reset(void)
{
	HAL_GPIO_WritePin(DCMI_RST_GPIO_Port, DCMI_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
    HAL_GPIO_WritePin(DCMI_RST_GPIO_Port, DCMI_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(20);
}

/**
 * @brief       OV5640模块软件复位
 * @param       无
 * @retval      无
 */
static void ov5640_sw_reset(void)
{
    uint8_t reg3103;
    reg3103 = ov5640_read_reg(0x3103);
    reg3103 &= ~(0x01 << 1);
    ov5640_write_reg(0x3103, reg3103);
    ov5640_write_reg(0x3008, 0x80);
    HAL_Delay(10);
}

/**
 * @brief       获取OV5640模块芯片ID
 * @param       无
 * @retval      芯片ID
 */
static uint16_t ov5640_get_chip_id(void)
{
    uint16_t chip_id;

    chip_id = ov5640_read_reg(0x300A) << 8;
    chip_id |= ov5640_read_reg(0x300B);

    return chip_id;
}

/**
 * @brief       初始化OV5640寄存器配置
 * @param       无
 * @retval      无
 */
static void ov5640_init_reg(void)
{
    uint32_t cfg_index;

    for (cfg_index=0; cfg_index<sizeof(ov5640_init_cfg)/sizeof(ov5640_reg_cfg_t); cfg_index++)
    {
    	if(ov5640_init_cfg[cfg_index].reg==0&&ov5640_init_cfg[cfg_index].dat==0)
    	{
    		continue;
    	}
        ov5640_write_reg(ov5640_init_cfg[cfg_index].reg, ov5640_init_cfg[cfg_index].dat);
        HAL_Delay(1);

    }
}

/**
 * @brief       获取OV5640模块ISP输入窗口尺寸
 * @param       无
 * @retval      无
 */
static void ov5640_get_isp_input_size(void)
{
    uint8_t reg3800;
    uint8_t reg3801;
    uint8_t reg3802;
    uint8_t reg3803;
    uint8_t reg3804;
    uint8_t reg3805;
    uint8_t reg3806;
    uint8_t reg3807;
    uint16_t x_addr_st;
    uint16_t y_addr_st;
    uint16_t x_addr_end;
    uint16_t y_addr_end;

    HAL_Delay(100);

    reg3800 = ov5640_read_reg(0x3800);
    reg3801 = ov5640_read_reg(0x3801);
    reg3802 = ov5640_read_reg(0x3802);
    reg3803 = ov5640_read_reg(0x3803);
    reg3804 = ov5640_read_reg(0x3804);
    reg3805 = ov5640_read_reg(0x3805);
    reg3806 = ov5640_read_reg(0x3806);
    reg3807 = ov5640_read_reg(0x3807);

    x_addr_st = (uint16_t)((reg3800 & 0x0F) << 8) | reg3801;
    y_addr_st = (uint16_t)((reg3802 & 0x07) << 8) | reg3803;
    x_addr_end = (uint16_t)((reg3804 & 0x0F) << 8) | reg3805;
    y_addr_end = (uint16_t)((reg3806 & 0x07) << 8) | reg3807;

    ov5640_sta.isp_input.width = x_addr_end - x_addr_st;
    ov5640_sta.isp_input.height = y_addr_end - y_addr_st;
}

/**
 * @brief       获取OV5640模块预缩放窗口尺寸
 * @param       无
 * @retval      无
 */
static void ov5640_get_pre_scaling_size(void)
{
    uint8_t reg3810;
    uint8_t reg3811;
    uint8_t reg3812;
    uint8_t reg3813;
    uint16_t x_offset;
    uint16_t y_offset;

    HAL_Delay(100);

    reg3810 = ov5640_read_reg(0x3810);
    reg3811 = ov5640_read_reg(0x3811);
    reg3812 = ov5640_read_reg(0x3812);
    reg3813 = ov5640_read_reg(0x3813);

    x_offset = (uint16_t)((reg3810 & 0x0F) << 8) | reg3811;
    y_offset = (uint16_t)((reg3812 & 0x07) << 8) | reg3813;

    ov5640_get_isp_input_size();

    ov5640_sta.pre_scaling.width = ov5640_sta.isp_input.width - (x_offset << 1);
    ov5640_sta.pre_scaling.height = ov5640_sta.isp_input.height - (y_offset << 1);
}

/**
 * @brief       获取OV5640模块输出图像尺寸
 * @param       无
 * @retval      无
 */
static void ov5640_get_output_size(void)
{
    uint8_t reg3808;
    uint8_t reg3809;
    uint8_t reg380A;
    uint8_t reg380B;
    uint16_t x_output_size;
    uint16_t y_output_size;

    HAL_Delay(100);

    reg3808 = ov5640_read_reg(0x3808);
    reg3809 = ov5640_read_reg(0x3809);
    reg380A = ov5640_read_reg(0x380A);
    reg380B = ov5640_read_reg(0x380B);

    x_output_size = (uint16_t)((reg3808 & 0x0F) << 8) | reg3809;
    y_output_size = (uint16_t)((reg380A & 0x07) << 8) | reg380B;

    ov5640_sta.output.width = x_output_size;
    ov5640_sta.output.height = y_output_size;
}

/**
 * @brief       初始化OV5640模块自动对焦
 * @param       无
 * @retval      OV5640_EOK     : OV5640模块自动对焦初始化成功
 *              OV5640_ETIMEOUT: OV5640模块下载固件超时
 */
uint8_t ov5640_auto_focus_init(void)
{
    uint32_t fw_index;
    uint16_t addr_index;
    uint8_t reg3029 = 0;
    uint16_t timeout = 0;

    ov5640_write_reg(0x3000, 0x20);

    for (addr_index=OV5640_FW_DOWNLOAD_ADDR, fw_index=0; fw_index<sizeof(ov5640_auto_focus_firmware);addr_index++, fw_index++)
    {
        ov5640_write_reg(addr_index, ov5640_auto_focus_firmware[fw_index]);
    }

    ov5640_write_reg(0x3022, 0x00);
    ov5640_write_reg(0x3023, 0x00);
    ov5640_write_reg(0x3024, 0x00);
    ov5640_write_reg(0x3025, 0x00);
    ov5640_write_reg(0x3026, 0x00);
    ov5640_write_reg(0x3027, 0x00);
    ov5640_write_reg(0x3028, 0x00);
    ov5640_write_reg(0x3029, 0x7F);
    ov5640_write_reg(0x3000, 0x00);

    while ((reg3029 != 0x70) && (timeout < OV5640_TIMEOUT))
    {
    	HAL_Delay(1);
        reg3029 = ov5640_read_reg(0x3029);
        timeout++;
    }

    if (reg3029 != 0x70)
    {
        return OV5640_ETIMEOUT;
    }

    return OV5640_EOK;
}

/**
 * @brief       OV5640模块自动对焦一次
 * @param       无
 * @retval      OV5640_EOK     : OV5640模块自动对焦一次成功
 *              OV5640_ETIMEOUT: OV5640模块自动对焦一次超时
 */
uint8_t ov5640_auto_focus_once(void)
{
    uint8_t reg3029 = 0;
    uint16_t timeout = 0;

    ov5640_write_reg(0x3022, 0x03);

    while ((reg3029 != 0x10) && (timeout < OV5640_TIMEOUT))
    {
    	HAL_Delay(1);
        reg3029 = ov5640_read_reg(0x3029);
        timeout++;
    }

    if (reg3029 != 0x10)
    {
        return OV5640_ETIMEOUT;
    }

    return OV5640_EOK;
}

/**
 * @brief       OV5640模块持续自动对焦
 * @param       无
 * @retval      OV5640_EOK     : OV5640模块持续自动对焦成功
 *              OV5640_ETIMEOUT: OV5640模块持续自动对焦超时
 */
uint8_t ov5640_auto_focus_continuance(void)
{
    uint8_t reg3023 = ~0;
    uint16_t timeout = 0;

    ov5640_write_reg(0x3023, 0x01);
    ov5640_write_reg(0x3022, 0x08);

    while ((reg3023 != 0x00) && (timeout < OV5640_TIMEOUT))
    {
    	HAL_Delay(1);
        reg3023 = ov5640_read_reg(0x3023);
        timeout++;
    }

    if (reg3023 != 0x00)
    {
        return OV5640_ETIMEOUT;
    }

    reg3023 = ~0;
    timeout = 0;

    ov5640_write_reg(0x3023, 0x01);
    ov5640_write_reg(0x3022, 0x04);

    while ((reg3023 != 0x00) && (timeout < OV5640_TIMEOUT))
    {
    	HAL_Delay(1);
        reg3023 = ov5640_read_reg(0x3023);
        timeout++;
    }

    if (reg3023 != 0x00)
    {
        return OV5640_ETIMEOUT;
    }

    return OV5640_EOK;
}

/**
 * @brief       开启OV5640模块闪光灯
 * @param       无
 * @retval      无
 */
void ov5640_led_on(void)
{
    ov5640_write_reg(0x3016, 0x02);
    ov5640_write_reg(0x301C, 0x02);
    ov5640_write_reg(0x3019, 0x02);

}

/**
 * @brief       关闭OV5640模块闪光灯
 * @param       无
 * @retval      无
 */
void ov5640_led_off(void)
{
    ov5640_write_reg(0x3016, 0x02);
    ov5640_write_reg(0x301C, 0x02);
    ov5640_write_reg(0x3019, 0x00);
}


/**
 * @brief       设置OV5640模块灯光模式
 * @param       mode: OV5640_LIGHT_MODE_ADVANCED_AWB : Advanced AWB
 *                    OV5640_LIGHT_MODE_SIMPLE_AWB   : Simple AWB
 *                    OV5640_LIGHT_MODE_MANUAL_DAY   : Manual day
 *                    OV5640_LIGHT_MODE_MANUAL_A     : Manual A
 *                    OV5640_LIGHT_MODE_MANUAL_CWF   : Manual cwf
 *                    OV5640_LIGHT_MODE_MANUAL_CLOUDY: Manual cloudy
 * @retval      OV5640_EOK   : 设置OV5640模块灯光模式成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_light_mode(ov5640_light_mode_t mode)
{
    switch (mode)
    {
        case OV5640_LIGHT_MODE_ADVANCED_AWB:
        {
            ov5640_write_reg(0x3406, 0x00);
            ov5640_write_reg(0x5192, 0x04);
            ov5640_write_reg(0x5191, 0xF8);
            ov5640_write_reg(0x5193, 0x70);
            ov5640_write_reg(0x5194, 0xF0);
            ov5640_write_reg(0x5195, 0xF0);
            ov5640_write_reg(0x518D, 0x3D);
            ov5640_write_reg(0x518F, 0x54);
            ov5640_write_reg(0x518E, 0x3D);
            ov5640_write_reg(0x5190, 0x54);
            ov5640_write_reg(0x518B, 0xA8);
            ov5640_write_reg(0x518C, 0xA8);
            ov5640_write_reg(0x5187, 0x18);
            ov5640_write_reg(0x5188, 0x18);
            ov5640_write_reg(0x5189, 0x6E);
            ov5640_write_reg(0x518A, 0x68);
            ov5640_write_reg(0x5186, 0x1C);
            ov5640_write_reg(0x5181, 0x50);
            ov5640_write_reg(0x5184, 0x25);
            ov5640_write_reg(0x5182, 0x11);
            ov5640_write_reg(0x5183, 0x14);
            ov5640_write_reg(0x5184, 0x25);
            ov5640_write_reg(0x5185, 0x24);
            break;
        }
        case OV5640_LIGHT_MODE_SIMPLE_AWB:
        {
            ov5640_write_reg(0x3406, 0x00);
            ov5640_write_reg(0x5183, 0x94);
            ov5640_write_reg(0x5191, 0xFF);
            ov5640_write_reg(0x5192, 0x00);
            break;
        }
        case OV5640_LIGHT_MODE_MANUAL_DAY:
        {
            ov5640_write_reg(0x3406, 0x01);
            ov5640_write_reg(0x3400, 0x06);
            ov5640_write_reg(0x3401, 0x1C);
            ov5640_write_reg(0x3402, 0x04);
            ov5640_write_reg(0x3403, 0x00);
            ov5640_write_reg(0x3404, 0x04);
            ov5640_write_reg(0x3405, 0xF3);
            break;
        }
        case OV5640_LIGHT_MODE_MANUAL_A:
        {
            ov5640_write_reg(0x3406, 0x01);
            ov5640_write_reg(0x3400, 0x04);
            ov5640_write_reg(0x3401, 0x10);
            ov5640_write_reg(0x3402, 0x04);
            ov5640_write_reg(0x3403, 0x00);
            ov5640_write_reg(0x3404, 0x08);
            ov5640_write_reg(0x3405, 0xB6);
            break;
        }
        case OV5640_LIGHT_MODE_MANUAL_CWF:
        {
            ov5640_write_reg(0x3406, 0x01);
            ov5640_write_reg(0x3400, 0x05);
            ov5640_write_reg(0x3401, 0x48);
            ov5640_write_reg(0x3402, 0x04);
            ov5640_write_reg(0x3403, 0x00);
            ov5640_write_reg(0x3404, 0x07);
            ov5640_write_reg(0x3405, 0xCF);
            break;
        }
        case OV5640_LIGHT_MODE_MANUAL_CLOUDY:
        {
            ov5640_write_reg(0x3406, 0x01);
            ov5640_write_reg(0x3400, 0x06);
            ov5640_write_reg(0x3401, 0x48);
            ov5640_write_reg(0x3402, 0x04);
            ov5640_write_reg(0x3403, 0x00);
            ov5640_write_reg(0x3404, 0x04);
            ov5640_write_reg(0x3405, 0xD3);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块色彩饱和度
 * @param       saturation: OV5640_COLOR_SATURATION_0: +4
 *                          OV5640_COLOR_SATURATION_1: +3
 *                          OV5640_COLOR_SATURATION_2: +2
 *                          OV5640_COLOR_SATURATION_3: +1
 *                          OV5640_COLOR_SATURATION_4: 0
 *                          OV5640_COLOR_SATURATION_5: -1
 *                          OV5640_COLOR_SATURATION_6: -2
 *                          OV5640_COLOR_SATURATION_7: -3
 *                          OV5640_COLOR_SATURATION_8: -4
 * @retval      OV5640_EOK   : 设置OV5640模块色彩饱和度成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_color_saturation(ov5640_color_saturation_t saturation)
{
    switch (saturation)
    {
        case OV5640_COLOR_SATURATION_0:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x80);
            ov5640_write_reg(0x5584, 0x80);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_1:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x70);
            ov5640_write_reg(0x5584, 0x70);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_2:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x60);
            ov5640_write_reg(0x5584, 0x60);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_3:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x50);
            ov5640_write_reg(0x5584, 0x50);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_4:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x40);
            ov5640_write_reg(0x5584, 0x40);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_5:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x30);
            ov5640_write_reg(0x5584, 0x30);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_6:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x20);
            ov5640_write_reg(0x5584, 0x20);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_7:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x10);
            ov5640_write_reg(0x5584, 0x10);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_COLOR_SATURATION_8:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5583, 0x00);
            ov5640_write_reg(0x5584, 0x00);
            ov5640_write_reg(0x5580, 0x02);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块亮度
 * @param       brightness: OV5640_BRIGHTNESS_0: +4
 *                          OV5640_BRIGHTNESS_1: +3
 *                          OV5640_BRIGHTNESS_2: +2
 *                          OV5640_BRIGHTNESS_3: +1
 *                          OV5640_BRIGHTNESS_4: 0
 *                          OV5640_BRIGHTNESS_5: -1
 *                          OV5640_BRIGHTNESS_6: -2
 *                          OV5640_BRIGHTNESS_7: -3
 *                          OV5640_BRIGHTNESS_8: -4
 * @retval      OV5640_EOK   : 设置OV5640模块亮度成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_brightness(ov5640_brightness_t brightness)
{
    switch (brightness)
    {
        case OV5640_BRIGHTNESS_0:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x40);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_BRIGHTNESS_1:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x30);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_BRIGHTNESS_2:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x20);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_BRIGHTNESS_3:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x10);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_BRIGHTNESS_4:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x00);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_BRIGHTNESS_5:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x10);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x09);
            break;
        }
        case OV5640_BRIGHTNESS_6:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x20);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x09);
            break;
        }
        case OV5640_BRIGHTNESS_7:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x30);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x09);
            break;
        }
        case OV5640_BRIGHTNESS_8:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5587, 0x40);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5588, 0x09);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块对比度
 * @param       contrast: OV5640_CONTRAST_0: +4
 *                        OV5640_CONTRAST_1: +3
 *                        OV5640_CONTRAST_2: +2
 *                        OV5640_CONTRAST_3: +1
 *                        OV5640_CONTRAST_4: 0
 *                        OV5640_CONTRAST_5: -1
 *                        OV5640_CONTRAST_6: -2
 *                        OV5640_CONTRAST_7: -3
 *                        OV5640_CONTRAST_8: -4
 * @retval      OV5640_EOK   : 设置OV5640模块对比度成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_contrast(ov5640_contrast_t contrast)
{
    switch (contrast)
    {
        case OV5640_CONTRAST_0:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x30);
            ov5640_write_reg(0x5585, 0x30);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_1:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x2C);
            ov5640_write_reg(0x5585, 0x2C);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_2:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x28);
            ov5640_write_reg(0x5585, 0x28);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_3:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x24);
            ov5640_write_reg(0x5585, 0x24);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_4:
        {
        	ov5640_write_reg(0x3212, 0x03);
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x20);
            ov5640_write_reg(0x5585, 0x20);
            ov5640_write_reg(0x5588, 0x41);
            ov5640_write_reg(0x3212, 0x13);
            ov5640_write_reg(0x3212, 0xa3);
            break;
        }
        case OV5640_CONTRAST_5:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x1C);
            ov5640_write_reg(0x5585, 0x1C);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_6:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x18);
            ov5640_write_reg(0x5585, 0x18);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_7:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x14);
            ov5640_write_reg(0x5585, 0x14);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        case OV5640_CONTRAST_8:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x04);
            ov5640_write_reg(0x5586, 0x10);
            ov5640_write_reg(0x5585, 0x10);
            ov5640_write_reg(0x5588, 0x41);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块色相
 * @param       contrast: OV5640_HUE_0 : -180 degree
 *                        OV5640_HUE_1 : -150 degree
 *                        OV5640_HUE_2 : -120 degree
 *                        OV5640_HUE_3 : -90 degree
 *                        OV5640_HUE_4 : -60 degree
 *                        OV5640_HUE_5 : -30 degree
 *                        OV5640_HUE_6 : 0 degree
 *                        OV5640_HUE_7 : +30 degree
 *                        OV5640_HUE_8 : +60 degree
 *                        OV5640_HUE_9 : +90 degree
 *                        OV5640_HUE_10: +120 degree
 *                        OV5640_HUE_11: +150 degree
 * @retval      OV5640_EOK   : 设置OV5640模块色相成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_hue(ov5640_hue_t hue)
{
    switch (hue)
    {
        case OV5640_HUE_0:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x80);
            ov5640_write_reg(0x5582, 0x00);
            ov5640_write_reg(0x5588, 0x32);
            break;
        }
        case OV5640_HUE_1:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x6F);
            ov5640_write_reg(0x5582, 0x40);
            ov5640_write_reg(0x5588, 0x32);
            break;
        }
        case OV5640_HUE_2:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x40);
            ov5640_write_reg(0x5582, 0x6F);
            ov5640_write_reg(0x5588, 0x32);
            break;
        }
        case OV5640_HUE_3:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x00);
            ov5640_write_reg(0x5582, 0x80);
            ov5640_write_reg(0x5588, 0x02);
            break;
        }
        case OV5640_HUE_4:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x40);
            ov5640_write_reg(0x5582, 0x6F);
            ov5640_write_reg(0x5588, 0x02);
            break;
        }
        case OV5640_HUE_5:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x6F);
            ov5640_write_reg(0x5582, 0x40);
            ov5640_write_reg(0x5588, 0x02);
            break;
        }
        case OV5640_HUE_6:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x80);
            ov5640_write_reg(0x5582, 0x00);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_HUE_7:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x6F);
            ov5640_write_reg(0x5582, 0x40);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_HUE_8:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x40);
            ov5640_write_reg(0x5582, 0x6F);
            ov5640_write_reg(0x5588, 0x01);
            break;
        }
        case OV5640_HUE_9:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x00);
            ov5640_write_reg(0x5582, 0x80);
            ov5640_write_reg(0x5588, 0x31);
            break;
        }
        case OV5640_HUE_10:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x40);
            ov5640_write_reg(0x5582, 0x6F);
            ov5640_write_reg(0x5588, 0x31);
            break;
        }
        case OV5640_HUE_11:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x01);
            ov5640_write_reg(0x5581, 0x6F);
            ov5640_write_reg(0x5582, 0x40);
            ov5640_write_reg(0x5588, 0x31);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块特殊效果
 * @param       contrast: OV5640_SPECIAL_EFFECT_NORMAL  : Normal
 *                        OV5640_SPECIAL_EFFECT_BW      : B&W
 *                        OV5640_SPECIAL_EFFECT_BLUISH  : Bluish
 *                        OV5640_SPECIAL_EFFECT_SEPIA   : Sepia
 *                        OV5640_SPECIAL_EFFECT_REDDISH : Reddish
 *                        OV5640_SPECIAL_EFFECT_GREENISH: Greenish
 *                        OV5640_SPECIAL_EFFECT_NEGATIVE: Negative
 * @retval      OV5640_EOK   : 设置OV5640模块特殊效果成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_special_effect(ov5640_special_effect_t effect)
{
    switch (effect)
    {
        case OV5640_SPECIAL_EFFECT_NORMAL:
        {
            ov5640_write_reg(0x5001, 0x7F);
            ov5640_write_reg(0x5580, 0x00);
            break;
        }
        case OV5640_SPECIAL_EFFECT_BW:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x18);
            ov5640_write_reg(0x5583, 0x80);
            ov5640_write_reg(0x5584, 0x80);
            break;
        }
        case OV5640_SPECIAL_EFFECT_BLUISH:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x18);
            ov5640_write_reg(0x5583, 0xA0);
            ov5640_write_reg(0x5584, 0x40);
            break;
        }
        case OV5640_SPECIAL_EFFECT_SEPIA:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x18);
            ov5640_write_reg(0x5583, 0x40);
            ov5640_write_reg(0x5584, 0xA0);
            break;
        }
        case OV5640_SPECIAL_EFFECT_REDDISH:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x18);
            ov5640_write_reg(0x5583, 0x80);
            ov5640_write_reg(0x5584, 0xC0);
            break;
        }
        case OV5640_SPECIAL_EFFECT_GREENISH:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x18);
            ov5640_write_reg(0x5583, 0x60);
            ov5640_write_reg(0x5584, 0x60);
            break;
        }
        case OV5640_SPECIAL_EFFECT_NEGATIVE:
        {
            ov5640_write_reg(0x5001, 0xFF);
            ov5640_write_reg(0x5580, 0x40);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块曝光度
 * @param       contrast: OV5640_EXPOSURE_LEVEL_0 : -1.7EV
 *                        OV5640_EXPOSURE_LEVEL_1 : -1.3EV
 *                        OV5640_EXPOSURE_LEVEL_2 : -1.0EV
 *                        OV5640_EXPOSURE_LEVEL_3 : -0.7EV
 *                        OV5640_EXPOSURE_LEVEL_4 : -0.3EV
 *                        OV5640_EXPOSURE_LEVEL_5 : default
 *                        OV5640_EXPOSURE_LEVEL_6 : 0.3EV
 *                        OV5640_EXPOSURE_LEVEL_7 : 0.7EV
 *                        OV5640_EXPOSURE_LEVEL_8 : 1.0EV
 *                        OV5640_EXPOSURE_LEVEL_9 : 1.3EV
 *                        OV5640_EXPOSURE_LEVEL_10: 1.7EV
 * @retval      OV5640_EOK   : 设置OV5640模块曝光度成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_exposure_level(ov5640_exposure_level_t level)
{
    switch (level)
    {
        case OV5640_EXPOSURE_LEVEL_0:
        {
            ov5640_write_reg(0x3A0F, 0x10);
            ov5640_write_reg(0x3A10, 0x08);
            ov5640_write_reg(0x3A1B, 0x10);
            ov5640_write_reg(0x3A1E, 0x08);
            ov5640_write_reg(0x3A11, 0x20);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_1:
        {
            ov5640_write_reg(0x3A0F, 0x18);
            ov5640_write_reg(0x3A10, 0x10);
            ov5640_write_reg(0x3A1B, 0x18);
            ov5640_write_reg(0x3A1E, 0x10);
            ov5640_write_reg(0x3A11, 0x30);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_2:
        {
            ov5640_write_reg(0x3A0F, 0x20);
            ov5640_write_reg(0x3A10, 0x18);
            ov5640_write_reg(0x3A11, 0x41);
            ov5640_write_reg(0x3A1B, 0x20);
            ov5640_write_reg(0x3A1E, 0x18);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_3:
        {
            ov5640_write_reg(0x3A0F, 0x28);
            ov5640_write_reg(0x3A10, 0x20);
            ov5640_write_reg(0x3A11, 0x51);
            ov5640_write_reg(0x3A1B, 0x28);
            ov5640_write_reg(0x3A1E, 0x20);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_4:
        {
            ov5640_write_reg(0x3A0F, 0x30);
            ov5640_write_reg(0x3A10, 0x28);
            ov5640_write_reg(0x3A11, 0x61);
            ov5640_write_reg(0x3A1B, 0x30);
            ov5640_write_reg(0x3A1E, 0x28);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_5:
        {
            ov5640_write_reg(0x3A0F, 0x38);
            ov5640_write_reg(0x3A10, 0x30);
            ov5640_write_reg(0x3A11, 0x61);
            ov5640_write_reg(0x3A1B, 0x38);
            ov5640_write_reg(0x3A1E, 0x30);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_6:
        {
            ov5640_write_reg(0x3A0F, 0x40);
            ov5640_write_reg(0x3A10, 0x38);
            ov5640_write_reg(0x3A11, 0x71);
            ov5640_write_reg(0x3A1B, 0x40);
            ov5640_write_reg(0x3A1E, 0x38);
            ov5640_write_reg(0x3A1F, 0x10);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_7:
        {
            ov5640_write_reg(0x3A0F, 0x48);
            ov5640_write_reg(0x3A10, 0x40);
            ov5640_write_reg(0x3A11, 0x80);
            ov5640_write_reg(0x3A1B, 0x48);
            ov5640_write_reg(0x3A1E, 0x40);
            ov5640_write_reg(0x3A1F, 0x20);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_8:
        {
            ov5640_write_reg(0x3A0F, 0x50);
            ov5640_write_reg(0x3A10, 0x48);
            ov5640_write_reg(0x3A11, 0x90);
            ov5640_write_reg(0x3A1B, 0x50);
            ov5640_write_reg(0x3A1E, 0x48);
            ov5640_write_reg(0x3A1F, 0x20);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_9:
        {
            ov5640_write_reg(0x3A0F, 0x58);
            ov5640_write_reg(0x3A10, 0x50);
            ov5640_write_reg(0x3A11, 0x91);
            ov5640_write_reg(0x3A1B, 0x58);
            ov5640_write_reg(0x3A1E, 0x50);
            ov5640_write_reg(0x3A1F, 0x20);
            break;
        }
        case OV5640_EXPOSURE_LEVEL_10:
        {
            ov5640_write_reg(0x3A0F, 0x60);
            ov5640_write_reg(0x3A10, 0x58);
            ov5640_write_reg(0x3A11, 0xA0);
            ov5640_write_reg(0x3A1B, 0x60);
            ov5640_write_reg(0x3A1E, 0x58);
            ov5640_write_reg(0x3A1F, 0x20);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块锐度
 * @param       contrast: OV5640_SHARPNESS_OFF  : Sharpness OFF
 *                        OV5640_SHARPNESS_1    : Sharpness 1
 *                        OV5640_SHARPNESS_2    : Sharpness 2
 *                        OV5640_SHARPNESS_3    : Sharpness 3
 *                        OV5640_SHARPNESS_4    : Sharpness 4
 *                        OV5640_SHARPNESS_5    : Sharpness 5
 *                        OV5640_SHARPNESS_6    : Sharpness 6
 *                        OV5640_SHARPNESS_7    : Sharpness 7
 *                        OV5640_SHARPNESS_8    : Sharpness 8
 *                        OV5640_SHARPNESS_AUTO : Sharpness Auto
 * @retval      OV5640_EOK   : 设置OV5640模块锐度成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_sharpness_level(ov5640_sharpness_t sharpness)
{
    switch (sharpness)
    {
        case OV5640_SHARPNESS_OFF:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x00);
            break;
        }
        case OV5640_SHARPNESS_1:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x02);
            break;
        }
        case OV5640_SHARPNESS_2:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x04);
            break;
        }
        case OV5640_SHARPNESS_3:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x08);
            break;
        }
        case OV5640_SHARPNESS_4:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x0C);
            break;
        }
        case OV5640_SHARPNESS_5:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x10);
            break;
        }
        case OV5640_SHARPNESS_6:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x14);
            break;
        }
        case OV5640_SHARPNESS_7:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x18);
            break;
        }
        case OV5640_SHARPNESS_8:
        {
            ov5640_write_reg(0x5308, 0x65);
            ov5640_write_reg(0x5302, 0x20);
            break;
        }
        case OV5640_SHARPNESS_AUTO:
        {
            ov5640_write_reg(0x5308, 0x25);
            ov5640_write_reg(0x5300, 0x08);
            ov5640_write_reg(0x5301, 0x30);
            ov5640_write_reg(0x5302, 0x10);
            ov5640_write_reg(0x5303, 0x00);
            ov5640_write_reg(0x5309, 0x08);
            ov5640_write_reg(0x530A, 0x30);
            ov5640_write_reg(0x530B, 0x04);
            ov5640_write_reg(0x530C, 0x06);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块镜像/翻转
 * @param       contrast: OV5640_MIRROR_FLIP_0: MIRROR
 *                        OV5640_MIRROR_FLIP_1: FLIP
 *                        OV5640_MIRROR_FLIP_2: MIRROR & FLIP
 *                        OV5640_MIRROR_FLIP_3: Normal
 * @retval      OV5640_EOK   : 设置OV5640模块镜像/翻转成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_mirror_flip(ov5640_mirror_flip_t mirror_flip)
{
    uint8_t reg3820;
    uint8_t reg3821;

    switch (mirror_flip)
    {
        case OV5640_MIRROR_FLIP_0:  //镜像
        {
            reg3820 = ov5640_read_reg(0x3820);
            reg3820 = reg3820 & 0xF9;
//            reg3820 = reg3820 | 0x00;
            ov5640_write_reg(0x3820, reg3820);
            reg3821 = ov5640_read_reg(0x3821);
//            reg3821 = reg3821 & 0xF9;
            reg3821 = reg3821 | 0x06;
            ov5640_write_reg(0x3821, reg3821);
            break;
        }
        case OV5640_MIRROR_FLIP_1: //翻转
        {
            reg3820 = ov5640_read_reg(0x3820);
//            reg3820 = reg3820 & 0xF9;
            reg3820 = reg3820 | 0x06;
            ov5640_write_reg(0x3820, reg3820);
            reg3821 = ov5640_read_reg(0x3821);
            reg3821 = reg3821 & 0xF9;
//            reg3821 = reg3821 | 0x00;
            ov5640_write_reg(0x3821, reg3821);
            break;
        }
        case OV5640_MIRROR_FLIP_2: //镜像+翻转
        {
            reg3820 = ov5640_read_reg(0x3820);
//            reg3820 = reg3820 & 0xF9;
            reg3820 = reg3820 | 0x06;
            ov5640_write_reg(0x3820, reg3820);
            reg3821 = ov5640_read_reg(0x3821);
//            reg3821 = reg3821 & 0xF9;
            reg3821 = reg3821 | 0x06;
            ov5640_write_reg(0x3821, reg3821);
            break;
        }
        case OV5640_MIRROR_FLIP_3: //正常
        {
            reg3820 = ov5640_read_reg(0x3820);
            reg3820 = reg3820 & 0xF9;
//            reg3820 = reg3820 | 0x00;
            ov5640_write_reg(0x3820, reg3820);
            reg3821 = ov5640_read_reg(0x3821);
            reg3821 = reg3821 & 0xF9;
//            reg3821 = reg3821 | 0x00;
            ov5640_write_reg(0x3821, reg3821);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块测试图案
 * @param       contrast: OV5640_TEST_PATTERN_OFF         : OFF
 *                        OV5640_TEST_PATTERN_COLOR_BAR   : Color bar
 *                        OV5640_TEST_PATTERN_COLOR_SQUARE: Color square
 * @retval      OV5640_EOK   : 设置OV5640模块测试图案成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_test_pattern(ov5640_test_pattern_t pattern)
{
    switch (pattern)
    {
        case OV5640_TEST_PATTERN_OFF:
        {
            ov5640_write_reg(0x503D, 0x00);
            ov5640_write_reg(0x4741, 0x00);
            break;
        }
        case OV5640_TEST_PATTERN_COLOR_BAR:
        {
            ov5640_write_reg(0x503D, 0x80);
            ov5640_write_reg(0x4741, 0x00);
            break;
        }
        case OV5640_TEST_PATTERN_COLOR_SQUARE:
        {
            ov5640_write_reg(0x503D, 0x82);
            ov5640_write_reg(0x4741, 0x00);
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块输出图像格式
 * @param       mode: OV5640_OUTPUT_FORMAT_RGB565: RGB565
 *                    OV5640_OUTPUT_FORMAT_JPEG  : JPEG
 * @retval      OV5640_EOK   : 设置OV5640模块输出图像格式成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_output_format(ov5640_output_format_t format)
{
    uint32_t cfg_index;

    switch (format)
    {
        case OV5640_OUTPUT_FORMAT_RGB565:
        {
            for (cfg_index=0; cfg_index<sizeof(ov5640_rgb565_cfg)/sizeof(ov5640_reg_cfg_t); cfg_index++)
            {
            	if(ov5640_rgb565_cfg[cfg_index].reg==0&&ov5640_rgb565_cfg[cfg_index].dat==0)
            	{
            		continue;
            	}
                ov5640_write_reg(ov5640_rgb565_cfg[cfg_index].reg, ov5640_rgb565_cfg[cfg_index].dat);
            }
            break;
        }
        case OV5640_OUTPUT_FORMAT_JPEG:
        {
            for (cfg_index=0; cfg_index<sizeof(ov5640_jpeg_cfg)/sizeof(ov5640_reg_cfg_t); cfg_index++)
            {
            	if(ov5640_jpeg_cfg[cfg_index].reg==0&&ov5640_jpeg_cfg[cfg_index].dat==0)
            	{
            		continue;
            	}
                ov5640_write_reg(ov5640_jpeg_cfg[cfg_index].reg, ov5640_jpeg_cfg[cfg_index].dat);
            }
            break;
        }
        default:
        {
            return OV5640_EINVAL;
        }
    }

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块ISP输入窗口尺寸
 * @note        OV5640模块ISP输入窗口的最大尺寸为0x0A3F*0x079F
 * @param       x     : ISP输入窗口起始X坐标
 *              y     : ISP输入窗口起始Y坐标
 *              width : ISP输入窗口宽度
 *              height: ISP输入窗口高度
 * @retval      OV5640_EOK   : 设置OV5640模块ISP输入窗口尺寸成功
 *              OV5640_EINVAL: 传入参数错误
 */
uint8_t ov5640_set_isp_input_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint8_t reg3800;
    uint8_t reg3801;
    uint8_t reg3802;
    uint8_t reg3803;
    uint8_t reg3804;
    uint8_t reg3805;
    uint8_t reg3806;
    uint8_t reg3807;
    uint16_t x_end;
    uint16_t y_end;

    x_end = x + width;
    y_end = y + height;

    if ((x_end > OV5640_ISP_INPUT_WIDTH_MAX) || (y_end > OV5640_ISP_INPUT_HEIGHT_MAX))
    {
        return OV5640_EINVAL;
    }

    reg3800 = ov5640_read_reg(0x3800);
    reg3802 = ov5640_read_reg(0x3802);
    reg3804 = ov5640_read_reg(0x3804);
    reg3806 = ov5640_read_reg(0x3806);

    reg3800 &= ~0x0F;
    reg3800 |= (uint8_t)(x >> 8) & 0x0F;
    reg3801 = (uint8_t)x & 0xFF;
    reg3802 &= ~0x0F;
    reg3802 |= (uint8_t)(y >> 8) & 0x0F;
    reg3803 = (uint8_t)y & 0xFF;
    reg3804 &= ~0x0F;
    reg3804 |= (uint8_t)(x_end >> 8) & 0x0F;
    reg3805 = (uint8_t)x_end & 0xFF;
    reg3806 &= ~0x07;
    reg3806 |= (uint8_t)(y_end >> 8) & 0x07;
    reg3807 = (uint8_t)y_end & 0xFF;

    ov5640_write_reg(0x3212, 0x03);
    ov5640_write_reg(0x3800, reg3800);
    ov5640_write_reg(0x3801, reg3801);
    ov5640_write_reg(0x3802, reg3802);
    ov5640_write_reg(0x3803, reg3803);
    ov5640_write_reg(0x3804, reg3804);
    ov5640_write_reg(0x3805, reg3805);
    ov5640_write_reg(0x3806, reg3806);
    ov5640_write_reg(0x3807, reg3807);
    ov5640_write_reg(0x3212, 0x13);
    ov5640_write_reg(0x3212, 0xA3);

    ov5640_get_isp_input_size();

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块预缩放窗口偏移
 * @note        OV5640模块预缩放窗口尺寸必须小于ISP输入窗口尺寸
 * @param       x_offset: 预缩放窗口X偏移
 *              y_offset: 预缩放窗口Y偏移
 * @retval      OV5640_EOK   : 设置OV5640模块预缩放窗口偏移成功
 */
uint8_t ov5640_set_pre_scaling_window(uint16_t x_offset, uint16_t y_offset)
{
    uint8_t reg3810;
    uint8_t reg3811;
    uint8_t reg3812;
    uint8_t reg3813;

    reg3810 = (uint8_t)(x_offset >> 8) & 0x0F;
    reg3811 = (uint8_t)x_offset & 0xFF;
    reg3812 = (uint8_t)(y_offset >> 8) & 0x07;
    reg3813 = (uint8_t)y_offset & 0xFF;

    ov5640_write_reg(0x3212, 0x03);
    ov5640_write_reg(0x3810, reg3810);
    ov5640_write_reg(0x3811, reg3811);
    ov5640_write_reg(0x3812, reg3812);
    ov5640_write_reg(0x3813, reg3813);
    ov5640_write_reg(0x3212, 0x13);
    ov5640_write_reg(0x3212, 0xA3);

    ov5640_get_pre_scaling_size();

    return OV5640_EOK;
}

/**
 * @brief       设置OV5640模块输出图像尺寸
 * @param       width : 实际输出图像的宽度（可能被缩放）
 *              height: 实际输出图像的高度（可能被缩放）
 * @retval      OV5640_EOK   : 设置OV5640模块输出图像窗口成功
 */
uint8_t ov5640_set_output_size(uint16_t width, uint16_t height)
{
    uint8_t reg3808;
    uint8_t reg3809;
    uint8_t reg380A;
    uint8_t reg380B;

    reg3808 = ov5640_read_reg(0x3808);
    reg380A = ov5640_read_reg(0x380A);

    reg3808 &= ~0x0F;
    reg3808 |= (uint8_t)(width >> 8) & 0x0F;
    reg3809 = (uint8_t)width & 0xFF;
    reg380A &= ~0x07;
    reg380A |= (uint8_t)(height >> 8) & 0x07;
    reg380B = (uint8_t)height & 0xFF;

    ov5640_write_reg(0x3212, 0x03);
    ov5640_write_reg(0x3808, reg3808);
    ov5640_write_reg(0x3809, reg3809);
    ov5640_write_reg(0x380A, reg380A);
    ov5640_write_reg(0x380B, reg380B);
    ov5640_write_reg(0x3212, 0x13);
    ov5640_write_reg(0x3212, 0xA3);

    ov5640_get_output_size();

    return OV5640_EOK;
}

/**
 * @brief       初始化OV5640模块
 * @param       无
 * @retval      OV5640_EOK   : OV5640模块初始化成功
 *              OV5640_ERROR : 通讯出错，OV5640模块初始化失败
 *              OV5640_ENOMEM: 内存不足，OV5640模块初始化失败
 */
uint8_t ov5640_init(void)
{
    uint16_t chip_id = 0x0000;

    ov5640_hw_init();               /* OV5640模块硬件初始化 */
    ov5640_exit_power_down();       /* OV5640模块退出掉电模式 */
    ov5640_hw_reset();              /* OV5640模块硬件复位 */
    ov5640_sw_reset();              /* OV5640模块软件复位 */

    chip_id = ov5640_get_chip_id(); /* 获取芯片ID */
    usb_printf("chip id is %x \r\n",chip_id);
    if (chip_id != OV5640_CHIP_ID)
    {
    	usb_printf("no ov5640 \r\n");
        return OV5640_ERROR;
    }

    ov5640_init_reg();              /* 初始化OV5640寄存器配置 */
    return OV5640_OK;
}

