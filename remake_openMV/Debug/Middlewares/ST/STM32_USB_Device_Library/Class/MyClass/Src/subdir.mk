################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.c 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/%.o Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/%.su Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/%.cyclo: ../Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/%.c Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/SPI_LCD" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"F:/STM32_camera/remake_openMV/source_code/remake_openMV/SD_FatFS" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MyClass-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MyClass-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.d ./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.o ./Middlewares/ST/STM32_USB_Device_Library/Class/MyClass/Src/usbd_mc.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MyClass-2f-Src

