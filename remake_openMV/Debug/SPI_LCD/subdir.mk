################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI_LCD/lcd_display.c \
../SPI_LCD/st7735.c 

OBJS += \
./SPI_LCD/lcd_display.o \
./SPI_LCD/st7735.o 

C_DEPS += \
./SPI_LCD/lcd_display.d \
./SPI_LCD/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
SPI_LCD/%.o SPI_LCD/%.su SPI_LCD/%.cyclo: ../SPI_LCD/%.c SPI_LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_USBD_COMPOSITE -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/SPI_LCD" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/SD_FatFS" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CompositeBuilder/Inc -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/user_bsp" -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/camera" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SPI_LCD

clean-SPI_LCD:
	-$(RM) ./SPI_LCD/lcd_display.cyclo ./SPI_LCD/lcd_display.d ./SPI_LCD/lcd_display.o ./SPI_LCD/lcd_display.su ./SPI_LCD/st7735.cyclo ./SPI_LCD/st7735.d ./SPI_LCD/st7735.o ./SPI_LCD/st7735.su

.PHONY: clean-SPI_LCD

