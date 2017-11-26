################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/fs/diskio.c \
../driver/fs/ff.c 

OBJS += \
./driver/fs/diskio.o \
./driver/fs/ff.o 

C_DEPS += \
./driver/fs/diskio.d \
./driver/fs/ff.d 


# Each subdirectory must supply rules for building sources it contributes
driver/fs/%.o: ../driver/fs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=arm920t -marm -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DREENTRANT_SYSCALLS_PROVIDED -I"F:\mini2440\work1\mini2440_rtos\bsp" -I"F:\mini2440\work1\mini2440_rtos\driver\fs" -I"F:\mini2440\work1\mini2440_rtos\bsp\sdio" -I"F:\mini2440\work1\mini2440_rtos\bsp\newlib" -I"F:\mini2440\work1\mini2440_rtos\bsp\newlib\sys" -I"F:\mini2440\work1\mini2440_rtos\bsp\dma" -I"F:\mini2440\work1\mini2440_rtos\driver" -I"F:\mini2440\work1\mini2440_rtos\bsp\interrupt" -I"F:\mini2440\work1\mini2440_rtos\bsp\uart" -I"F:\mini2440\work1\mini2440_rtos\bsp\clock" -I"F:\mini2440\work1\mini2440_rtos\bsp\common" -I"F:\mini2440\work1\mini2440_rtos\bsp\gpio" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


