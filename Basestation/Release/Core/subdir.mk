################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/HardwareSerial.cpp \
../Core/Print.cpp \
../Core/Tone.cpp \
../Core/WMath.cpp \
../Core/WString.cpp 

C_SRCS += \
../Core/WInterrupts.c \
../Core/pins_arduino.c \
../Core/wiring.c \
../Core/wiring_analog.c \
../Core/wiring_digital.c \
../Core/wiring_pulse.c \
../Core/wiring_shift.c 

OBJS += \
./Core/HardwareSerial.o \
./Core/Print.o \
./Core/Tone.o \
./Core/WInterrupts.o \
./Core/WMath.o \
./Core/WString.o \
./Core/pins_arduino.o \
./Core/wiring.o \
./Core/wiring_analog.o \
./Core/wiring_digital.o \
./Core/wiring_pulse.o \
./Core/wiring_shift.o 

C_DEPS += \
./Core/WInterrupts.d \
./Core/pins_arduino.d \
./Core/wiring.d \
./Core/wiring_analog.d \
./Core/wiring_digital.d \
./Core/wiring_pulse.d \
./Core/wiring_shift.d 

CPP_DEPS += \
./Core/HardwareSerial.d \
./Core/Print.d \
./Core/Tone.d \
./Core/WMath.d \
./Core/WString.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Wl,--gc-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Core/%.o: ../Core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


