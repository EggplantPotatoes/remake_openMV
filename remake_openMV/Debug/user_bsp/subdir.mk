################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_bsp/IIC_bus.c \
../user_bsp/usb_vcp.c 

OBJS += \
./user_bsp/IIC_bus.o \
./user_bsp/usb_vcp.o 

C_DEPS += \
./user_bsp/IIC_bus.d \
./user_bsp/usb_vcp.d 


# Each subdirectory must supply rules for building sources it contributes
user_bsp/%.o user_bsp/%.su user_bsp/%.cyclo: ../user_bsp/%.c user_bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_USBD_COMPOSITE -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"E:/Jundong/Git/remake_openMV/remake_openMV/SPI_LCD" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"E:/Jundong/Git/remake_openMV/remake_openMV/SD_FatFS" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CompositeBuilder/Inc -I"E:/Jundong/Git/remake_openMV/remake_openMV/user_usb" -I"E:/Jundong/Git/remake_openMV/remake_openMV/user_bsp" -I"E:/Jundong/Git/remake_openMV/remake_openMV/camera" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_bsp

clean-user_bsp:
	-$(RM) ./user_bsp/IIC_bus.cyclo ./user_bsp/IIC_bus.d ./user_bsp/IIC_bus.o ./user_bsp/IIC_bus.su ./user_bsp/usb_vcp.cyclo ./user_bsp/usb_vcp.d ./user_bsp/usb_vcp.o ./user_bsp/usb_vcp.su

.PHONY: clean-user_bsp

