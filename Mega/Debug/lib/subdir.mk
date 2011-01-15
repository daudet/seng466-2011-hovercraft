################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/HardwareSerial.cpp \
../lib/Print.cpp \
../lib/Tone.cpp \
../lib/WMath.cpp \
../lib/WString.cpp 

C_SRCS += \
../lib/WInterrupts.c \
../lib/pins_arduino.c \
../lib/wiring.c \
../lib/wiring_analog.c \
../lib/wiring_digital.c \
../lib/wiring_pulse.c \
../lib/wiring_shift.c 

OBJS += \
./lib/HardwareSerial.o \
./lib/Print.o \
./lib/Tone.o \
./lib/WInterrupts.o \
./lib/WMath.o \
./lib/WString.o \
./lib/pins_arduino.o \
./lib/wiring.o \
./lib/wiring_analog.o \
./lib/wiring_digital.o \
./lib/wiring_pulse.o \
./lib/wiring_shift.o 

C_DEPS += \
./lib/WInterrupts.d \
./lib/pins_arduino.d \
./lib/wiring.d \
./lib/wiring_analog.d \
./lib/wiring_digital.d \
./lib/wiring_pulse.d \
./lib/wiring_shift.d 

CPP_DEPS += \
./lib/HardwareSerial.d \
./lib/Print.d \
./lib/Tone.d \
./lib/WMath.d \
./lib/WString.d 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o: ../lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega1280 -DF_CPU=160000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/%.o: ../lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega1280 -DF_CPU=160000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


