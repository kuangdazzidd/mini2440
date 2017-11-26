################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/gpio.c 

OBJS += \
./bsp/gpio.o 

C_DEPS += \
./bsp/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o: ../bsp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm920t -marm -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"F:\mini2440\work1\mini2440_rtos\bsp" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


