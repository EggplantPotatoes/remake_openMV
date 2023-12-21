/*
 * sdmmc_driver.c
 *
 *  Created on: Dec 19, 2023
 *      Author: smile
 */

#include "sdmmc.h"
#include "sdmmc_driver.h"


//extern SD_HandleTypeDef hsd1;
HAL_SD_CardInfoTypeDef SDCardInfo;


void SD_Init(void)
{
	MX_SDMMC1_SD_Init();
}


//得到卡信息
//cardinfo:卡信息存储区
//返回值:错误状态
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
	sta=HAL_SD_GetCardInfo(&hsd1,cardinfo);
    return sta;
}

//判断SD卡是否可以传输(读写)数据
//返回值:SD_TRANSFER_OK 传输完成，可以继续下一次传输
//		 SD_TRANSFER_BUSY SD卡正忙，不可以进行下一次传输
uint8_t SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&hsd1)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数
//返回值:错误状态;0,正常;其他,错误代码;
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    __disable_irq();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&hsd1, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作

	//等待SD卡读完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }
	__enable_irq();//开启总中断
    return sta;
}


//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数
//返回值:错误状态;0,正常;其他,错误代码;
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    __disable_irq();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&hsd1,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作

	//等待SD卡写完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{
			sta=SD_TRANSFER_BUSY;
            break;
		}
    }
	__enable_irq();//开启总中断
    return sta;
}

