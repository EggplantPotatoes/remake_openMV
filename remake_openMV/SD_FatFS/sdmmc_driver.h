/*
 * sdmmc_driver.h
 *
 *  Created on: Dec 19, 2023
 *      Author: smile
 */

#ifndef SDMMC_DRIVER_H_
#define SDMMC_DRIVER_H_

#include "main.h"

#define SD_TIMEOUT      	((uint32_t)100000000)       //超时时间
#define SD_TRANSFER_OK     	((uint8_t)0x00)
#define SD_TRANSFER_BUSY   	((uint8_t)0x01)

extern SD_HandleTypeDef hsd1;
extern HAL_SD_CardInfoTypeDef SDCardInfo;

void SD_Init(void);
uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo);
uint8_t SD_GetCardState(void);
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt);
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);

#endif /* SDMMC_DRIVER_H_ */
