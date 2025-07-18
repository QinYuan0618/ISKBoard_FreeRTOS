################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/mycore/bsp_key.c \
../Core/Src/mycore/bsp_led.c \
../Core/Src/mycore/core.c \
../Core/Src/mycore/miscdev.c 

OBJS += \
./Core/Src/mycore/bsp_key.o \
./Core/Src/mycore/bsp_led.o \
./Core/Src/mycore/core.o \
./Core/Src/mycore/miscdev.o 

C_DEPS += \
./Core/Src/mycore/bsp_key.d \
./Core/Src/mycore/bsp_led.d \
./Core/Src/mycore/core.d \
./Core/Src/mycore/miscdev.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/mycore/%.o Core/Src/mycore/%.su Core/Src/mycore/%.cyclo: ../Core/Src/mycore/%.c Core/Src/mycore/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-mycore

clean-Core-2f-Src-2f-mycore:
	-$(RM) ./Core/Src/mycore/bsp_key.cyclo ./Core/Src/mycore/bsp_key.d ./Core/Src/mycore/bsp_key.o ./Core/Src/mycore/bsp_key.su ./Core/Src/mycore/bsp_led.cyclo ./Core/Src/mycore/bsp_led.d ./Core/Src/mycore/bsp_led.o ./Core/Src/mycore/bsp_led.su ./Core/Src/mycore/core.cyclo ./Core/Src/mycore/core.d ./Core/Src/mycore/core.o ./Core/Src/mycore/core.su ./Core/Src/mycore/miscdev.cyclo ./Core/Src/mycore/miscdev.d ./Core/Src/mycore/miscdev.o ./Core/Src/mycore/miscdev.su

.PHONY: clean-Core-2f-Src-2f-mycore

