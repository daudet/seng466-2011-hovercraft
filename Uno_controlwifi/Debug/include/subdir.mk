################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/HardwareSerial.cpp \
../include/Print.cpp \
../include/Tone.cpp \
../include/WMath.cpp \
../include/WString.cpp 

C_SRCS += \
../include/WInterrupts.c \
../include/pins_arduino.c \
../include/wiring.c \
../include/wiring_analog.c \
../include/wiring_digital.c \
../include/wiring_pulse.c \
../include/wiring_shift.c 

OBJS += \
./include/HardwareSerial.o \
./include/Print.o \
./include/Tone.o \
./include/WInterrupts.o \
./include/WMath.o \
./include/WString.o \
./include/pins_arduino.o \
./include/wiring.o \
./include/wiring_analog.o \
./include/wiring_digital.o \
./include/wiring_pulse.o \
./include/wiring_shift.o 

C_DEPS += \
./include/WInterrupts.d \
./include/pins_arduino.d \
./include/wiring.d \
./include/wiring_analog.d \
./include/wiring_digital.d \
./include/wiring_pulse.d \
./include/wiring_shift.d 

CPP_DEPS += \
./include/HardwareSerial.d \
./include/Print.d \
./include/Tone.d \
./include/WMath.d \
./include/WString.d 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"H:\seng466\Uno_controlwifi\include" -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/%.o: ../include/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"H:\seng466\Uno_controlwifi\include" -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


