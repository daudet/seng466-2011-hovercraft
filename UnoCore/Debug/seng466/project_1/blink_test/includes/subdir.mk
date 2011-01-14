################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../seng466/project_1/blink_test/includes/HardwareSerial.cpp \
../seng466/project_1/blink_test/includes/Print.cpp \
../seng466/project_1/blink_test/includes/Tone.cpp \
../seng466/project_1/blink_test/includes/WMath.cpp \
../seng466/project_1/blink_test/includes/WString.cpp 

C_SRCS += \
../seng466/project_1/blink_test/includes/WInterrupts.c \
../seng466/project_1/blink_test/includes/pins_arduino.c \
../seng466/project_1/blink_test/includes/wiring.c \
../seng466/project_1/blink_test/includes/wiring_analog.c \
../seng466/project_1/blink_test/includes/wiring_digital.c \
../seng466/project_1/blink_test/includes/wiring_pulse.c \
../seng466/project_1/blink_test/includes/wiring_shift.c 

OBJS += \
./seng466/project_1/blink_test/includes/HardwareSerial.o \
./seng466/project_1/blink_test/includes/Print.o \
./seng466/project_1/blink_test/includes/Tone.o \
./seng466/project_1/blink_test/includes/WInterrupts.o \
./seng466/project_1/blink_test/includes/WMath.o \
./seng466/project_1/blink_test/includes/WString.o \
./seng466/project_1/blink_test/includes/pins_arduino.o \
./seng466/project_1/blink_test/includes/wiring.o \
./seng466/project_1/blink_test/includes/wiring_analog.o \
./seng466/project_1/blink_test/includes/wiring_digital.o \
./seng466/project_1/blink_test/includes/wiring_pulse.o \
./seng466/project_1/blink_test/includes/wiring_shift.o 

C_DEPS += \
./seng466/project_1/blink_test/includes/WInterrupts.d \
./seng466/project_1/blink_test/includes/pins_arduino.d \
./seng466/project_1/blink_test/includes/wiring.d \
./seng466/project_1/blink_test/includes/wiring_analog.d \
./seng466/project_1/blink_test/includes/wiring_digital.d \
./seng466/project_1/blink_test/includes/wiring_pulse.d \
./seng466/project_1/blink_test/includes/wiring_shift.d 

CPP_DEPS += \
./seng466/project_1/blink_test/includes/HardwareSerial.d \
./seng466/project_1/blink_test/includes/Print.d \
./seng466/project_1/blink_test/includes/Tone.d \
./seng466/project_1/blink_test/includes/WMath.d \
./seng466/project_1/blink_test/includes/WString.d 


# Each subdirectory must supply rules for building sources it contributes
seng466/project_1/blink_test/includes/%.o: ../seng466/project_1/blink_test/includes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Wl,--gc-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

seng466/project_1/blink_test/includes/%.o: ../seng466/project_1/blink_test/includes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


