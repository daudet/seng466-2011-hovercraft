/*
 * UART.c
 *
 *  Created on: Jan 29, 2011
 *      Author: cei
 */

#include "UART.h"
#include "../Core/WProgram.h"

void UARTinit()
{
	Serial.begin(BAUD);
}

void initbuffer(byte* buffer, byte size)
{
	byte i;
	for(i = 0; i < size; i++)
		buffer[i] = 0;
}

int UARTreceive(byte* buffer, byte size)
{
	if (Serial.available() >= (size+2)){
		if (Serial.read()==PRE1){
			if (Serial.read()==PRE2){
				byte i;
				for (i = 0; i < size ; i++)
					buffer[i] = Serial.read();
				Serial.flush();
				return 1;
			}
		}
	}
	return 0;
}

void UARTsend(byte* buffer, byte size)
{
	byte PRE[2] = {PRE1,PRE2};
	Serial.write(PRE,2);
	Serial.write(buffer, size);
}

