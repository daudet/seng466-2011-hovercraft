################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Mega.lss \

FLASH_IMAGE += \
Mega.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Mega.elf secondary-outputs

# Tool invocations
Mega.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"Mega.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,Mega.map,--cref -L"H:\seng466\Mega\lib" -mmcu=atmega1280
	@echo 'Finished building target: $@'
	@echo ' '

Mega.lss: Mega.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Mega.elf  >"Mega.lss"
	@echo 'Finished building: $@'
	@echo ' '

Mega.hex: Mega.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex Mega.elf  "Mega.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Mega.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega1280 Mega.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) Mega.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
