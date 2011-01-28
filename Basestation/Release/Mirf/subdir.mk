################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Mirf/Mirf.cpp 

OBJS += \
./Mirf/Mirf.o 

CPP_DEPS += \
./Mirf/Mirf.d 


# Each subdirectory must supply rules for building sources it contributes
Mirf/%.o: ../Mirf/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/avr/include -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Wl,--gc-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


