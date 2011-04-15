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
	Serial1.begin(BAUD);
}

void initbuffer(byte* buffer, byte size)
{
	byte i;
	for(i = 0; i < size; i++)
		buffer[i] = 0;
}

int UARTreceive(byte* buffer, byte size)
{
	if (Serial1.available() >= (size+2)){
		if (Serial1.read()==PRE1){
			if (Serial1.read()==PRE2){
				byte i;
				for (i = 0; i < size ; i++)
					buffer[i] = Serial1.read();
				Serial1.flush();
				return 1;
			}
		}
	}
	return 0;
}

void UARTsend(byte* buffer, byte size)
{
	byte PRE[2] = {PRE1,PRE2};
	Serial1.write(PRE,2);
	Serial1.write(buffer, size);
}

