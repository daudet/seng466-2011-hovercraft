/*
 * UART.h
 *
 *  Created on: Jan 29, 2011
 *      Author: cei
 */

#include "../Core/WProgram.h"


#define BAUD 9600
#define PRE1 0xFE
#define PRE2 0xFF


void UARTinit();
void initbuffer(byte*, byte);
int UARTreceive(byte*, byte);
void UARTsend(byte*, byte);

