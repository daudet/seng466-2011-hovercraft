/*
 * UART.h
 *
 *  Created on: Jan 29, 2011
 *      Author: cei
 */

#include "../Core/WProgram.h"

#define BAUD 38400
#define TO 400

void UARTinit();
void initbuffer(byte*, byte);
int UARTreceive(byte*, byte);
void UARTsend(byte*, byte);

