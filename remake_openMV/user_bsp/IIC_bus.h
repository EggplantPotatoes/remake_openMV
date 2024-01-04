/*
 * IIC_bus.h
 *
 *  Created on: Dec 7, 2023
 *      Author: smile
 */

#ifndef IIC_BUS_H_
#define IIC_BUS_H_

#include "main.h"

int32_t BSP_I2C2_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C2_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);

#endif /* IIC_BUS_H_ */
