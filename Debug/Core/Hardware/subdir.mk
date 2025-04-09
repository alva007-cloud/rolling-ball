################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Hardware/\ state_machine.c \
../Core/Hardware/ADC_pid.c \
../Core/Hardware/Encoder.c \
../Core/Hardware/Key.c \
../Core/Hardware/OLED.c \
../Core/Hardware/TOF050C.c \
../Core/Hardware/font.c \
../Core/Hardware/motor.c \
../Core/Hardware/pid.c \
../Core/Hardware/usart_with_zdt.c 

OBJS += \
./Core/Hardware/\ state_machine.o \
./Core/Hardware/ADC_pid.o \
./Core/Hardware/Encoder.o \
./Core/Hardware/Key.o \
./Core/Hardware/OLED.o \
./Core/Hardware/TOF050C.o \
./Core/Hardware/font.o \
./Core/Hardware/motor.o \
./Core/Hardware/pid.o \
./Core/Hardware/usart_with_zdt.o 

C_DEPS += \
./Core/Hardware/\ state_machine.d \
./Core/Hardware/ADC_pid.d \
./Core/Hardware/Encoder.d \
./Core/Hardware/Key.d \
./Core/Hardware/OLED.d \
./Core/Hardware/TOF050C.d \
./Core/Hardware/font.d \
./Core/Hardware/motor.d \
./Core/Hardware/pid.d \
./Core/Hardware/usart_with_zdt.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Hardware/\ state_machine.o: ../Core/Hardware/\ state_machine.c Core/Hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DVECT_TAB_SRAM -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32_project/motor/Core/Hardware" -I"D:/STM32_project/motor/Core/Library" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Hardware/ state_machine.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Hardware/%.o Core/Hardware/%.su Core/Hardware/%.cyclo: ../Core/Hardware/%.c Core/Hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DVECT_TAB_SRAM -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32_project/motor/Core/Hardware" -I"D:/STM32_project/motor/Core/Library" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Hardware

clean-Core-2f-Hardware:
	-$(RM) ./Core/Hardware/\ state_machine.cyclo ./Core/Hardware/\ state_machine.d ./Core/Hardware/\ state_machine.o ./Core/Hardware/\ state_machine.su ./Core/Hardware/ADC_pid.cyclo ./Core/Hardware/ADC_pid.d ./Core/Hardware/ADC_pid.o ./Core/Hardware/ADC_pid.su ./Core/Hardware/Encoder.cyclo ./Core/Hardware/Encoder.d ./Core/Hardware/Encoder.o ./Core/Hardware/Encoder.su ./Core/Hardware/Key.cyclo ./Core/Hardware/Key.d ./Core/Hardware/Key.o ./Core/Hardware/Key.su ./Core/Hardware/OLED.cyclo ./Core/Hardware/OLED.d ./Core/Hardware/OLED.o ./Core/Hardware/OLED.su ./Core/Hardware/TOF050C.cyclo ./Core/Hardware/TOF050C.d ./Core/Hardware/TOF050C.o ./Core/Hardware/TOF050C.su ./Core/Hardware/font.cyclo ./Core/Hardware/font.d ./Core/Hardware/font.o ./Core/Hardware/font.su ./Core/Hardware/motor.cyclo ./Core/Hardware/motor.d ./Core/Hardware/motor.o ./Core/Hardware/motor.su ./Core/Hardware/pid.cyclo ./Core/Hardware/pid.d ./Core/Hardware/pid.o ./Core/Hardware/pid.su ./Core/Hardware/usart_with_zdt.cyclo ./Core/Hardware/usart_with_zdt.d ./Core/Hardware/usart_with_zdt.o ./Core/Hardware/usart_with_zdt.su

.PHONY: clean-Core-2f-Hardware

