################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserApp/OV7725/ov7725.c \
../UserApp/OV7725/sccb.c 

C_DEPS += \
./UserApp/OV7725/ov7725.d \
./UserApp/OV7725/sccb.d 

OBJS += \
./UserApp/OV7725/ov7725.o \
./UserApp/OV7725/sccb.o 


# Each subdirectory must supply rules for building sources it contributes
UserApp/OV7725/%.o UserApp/OV7725/%.su: ../UserApp/OV7725/%.c UserApp/OV7725/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/Bsp/communication" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/Bsp/Utils" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/UserApp" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/UserApp/Lcd" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/Bsp/timer" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/UserApp/OV7725" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/UserApp/Signal" -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/Usmart" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"E:/CODE/OV7725/OV7725_cube_RTOSV1/UserApp/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-UserApp-2f-OV7725

clean-UserApp-2f-OV7725:
	-$(RM) ./UserApp/OV7725/ov7725.d ./UserApp/OV7725/ov7725.o ./UserApp/OV7725/ov7725.su ./UserApp/OV7725/sccb.d ./UserApp/OV7725/sccb.o ./UserApp/OV7725/sccb.su

.PHONY: clean-UserApp-2f-OV7725

