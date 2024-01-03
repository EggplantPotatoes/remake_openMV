################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_bsp/IIC_bus.c \
../user_bsp/usb_bsp.c 

OBJS += \
./user_bsp/IIC_bus.o \
./user_bsp/usb_bsp.o 

C_DEPS += \
./user_bsp/IIC_bus.d \
./user_bsp/usb_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
user_bsp/%.o user_bsp/%.su user_bsp/%.cyclo: ../user_bsp/%.c user_bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/user_bsp" -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/camera" -I"F:/STM32_camera/my_code/openmv4_plus_board_code_camera_lcd_ok/SPI_LCD" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_bsp

clean-user_bsp:
	-$(RM) ./user_bsp/IIC_bus.cyclo ./user_bsp/IIC_bus.d ./user_bsp/IIC_bus.o ./user_bsp/IIC_bus.su ./user_bsp/usb_bsp.cyclo ./user_bsp/usb_bsp.d ./user_bsp/usb_bsp.o ./user_bsp/usb_bsp.su

.PHONY: clean-user_bsp

