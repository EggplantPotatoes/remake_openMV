################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../camera/camera_run.c \
../camera/ov5640.c \
../camera/ov5640_cfg.c 

OBJS += \
./camera/camera_run.o \
./camera/ov5640.o \
./camera/ov5640_cfg.o 

C_DEPS += \
./camera/camera_run.d \
./camera/ov5640.d \
./camera/ov5640_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
camera/%.o camera/%.su camera/%.cyclo: ../camera/%.c camera/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_USBD_COMPOSITE -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"E:/Jundong/Git/remake_openMV/remake_openMV/SPI_LCD" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"E:/Jundong/Git/remake_openMV/remake_openMV/SD_FatFS" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CompositeBuilder/Inc -I"E:/Jundong/Git/remake_openMV/remake_openMV/user_usb" -I"E:/Jundong/Git/remake_openMV/remake_openMV/user_bsp" -I"E:/Jundong/Git/remake_openMV/remake_openMV/camera" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-camera

clean-camera:
	-$(RM) ./camera/camera_run.cyclo ./camera/camera_run.d ./camera/camera_run.o ./camera/camera_run.su ./camera/ov5640.cyclo ./camera/ov5640.d ./camera/ov5640.o ./camera/ov5640.su ./camera/ov5640_cfg.cyclo ./camera/ov5640_cfg.d ./camera/ov5640_cfg.o ./camera/ov5640_cfg.su

.PHONY: clean-camera

