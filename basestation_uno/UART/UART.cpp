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
	if (Serial.available() >= size)
	{
		byte i;
		for (i = 0; i < size ; i++)
			buffer[i] = Serial.read();
		Serial.flush();
		return 1;
	}
	return 0;
}

void UARTsend(byte* buffer, byte size)
{
	Serial.write(buffer, size);
}

