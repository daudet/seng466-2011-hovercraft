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

void UARTinitbuffer(byte* buffer)
{
	byte i = 0;
	for(i = 0; i < BUFFERSIZE; i++)
		buffer[i] = 0;
}

void UARTreceive(byte* buffer)
{
	byte i = 0;

	// wait until a full packet has been buffered
	while (Serial.available() < BUFFERSIZE);

	//fill the buffer with the bytes received
	for (i = 0; i < BUFFERSIZE ; i++)
		buffer[i] = Serial.read();
	Serial.flush();
}

void UARTsend(byte* buffer)
{
	Serial.write(buffer,BUFFERSIZE);
	UARTinitbuffer(buffer);
}

