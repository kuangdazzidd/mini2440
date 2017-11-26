################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include driver/fs/subdir.mk
-include driver/subdir.mk
-include bsp/uart/subdir.mk
-include bsp/sdio/subdir.mk
-include bsp/newlib/subdir.mk
-include bsp/interrupt/subdir.mk
-include bsp/gpio/subdir.mk
-include bsp/dma/subdir.mk
-include bsp/clock/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
mini2440_rtos.hex \

SECONDARY_SIZE += \
mini2440_rtos.siz \


# All Target
all: mini2440_rtos.elf secondary-outputs

# Tool invocations
mini2440_rtos.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=arm920t -marm -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "F:\mini2440\work1\mini2440_rtos\mini2440.ld" -nostartfiles -Xlinker --gc-sections -L"C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q3\arm-none-eabi\lib" -Wl,-Map,"mini2440_rtos.map" -o "mini2440_rtos.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

mini2440_rtos.hex: mini2440_rtos.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "mini2440_rtos.elf"  "mini2440_rtos.hex"
	@echo 'Finished building: $@'
	@echo ' '

mini2440_rtos.siz: mini2440_rtos.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "mini2440_rtos.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) mini2440_rtos.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
