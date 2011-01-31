/*
 * UART.h
 *
 *  Created on: Jan 29, 2011
 *      Author: cei
 */

#include "../Core/WProgram.h"

#define BUFFERSIZE 4
#define BAUD 38400
#define TO 400

void UARTinit();
void UARTinitbuffer(byte*);
void UARTreceive(byte*);
void UARTsend(byte*);

