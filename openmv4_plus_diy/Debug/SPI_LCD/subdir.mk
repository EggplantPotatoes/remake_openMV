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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/user_bsp" -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/camera" -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/SPI_LCD" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SPI_LCD

clean-SPI_LCD:
	-$(RM) ./SPI_LCD/lcd_display.cyclo ./SPI_LCD/lcd_display.d ./SPI_LCD/lcd_display.o ./SPI_LCD/lcd_display.su ./SPI_LCD/st7735.cyclo ./SPI_LCD/st7735.d ./SPI_LCD/st7735.o ./SPI_LCD/st7735.su

.PHONY: clean-SPI_LCD

