################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HardwareSerial.cpp \
../Print.cpp \
../Tone.cpp \
../WMath.cpp \
../WString.cpp \
../main.cpp 

C_SRCS += \
../WInterrupts.c \
../pins_arduino.c \
../wiring.c \
../wiring_analog.c \
../wiring_digital.c \
../wiring_pulse.c \
../wiring_shift.c 

OBJS += \
./HardwareSerial.o \
./Print.o \
./Tone.o \
./WInterrupts.o \
./WMath.o \
./WString.o \
./main.o \
./pins_arduino.o \
./wiring.o \
./wiring_analog.o \
./wiring_digital.o \
./wiring_pulse.o \
./wiring_shift.o 

C_DEPS += \
./WInterrupts.d \
./pins_arduino.d \
./wiring.d \
./wiring_analog.d \
./wiring_digital.d \
./wiring_pulse.d \
./wiring_shift.d 

CPP_DEPS += \
./HardwareSerial.d \
./Print.d \
./Tone.d \
./WMath.d \
./WString.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega1280 -DF_CPU=160000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega1280 -DF_CPU=160000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


