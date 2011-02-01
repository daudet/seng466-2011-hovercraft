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
	byte i = 0;
	for(i = 0; i < size; i++)
		buffer[i] = 0;
}

void UARTreceive(byte* buffer, byte size)
{
	byte i = 0;
	unsigned long t=millis();
	// wait until a full packet has been buffered
	while (Serial.available() < size)
	{
		if ((millis()-t)>TO)
		{
			initbuffer(buffer, size);
			return;
		}
	}


	//fill the buffer with the bytes received
	for (i = 0; i < size ; i++)
		buffer[i] = Serial.read();
	Serial.flush();
}

void UARTsend(byte* buffer, byte size)
{
	Serial.write(buffer, size);
}

