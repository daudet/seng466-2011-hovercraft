/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "Core/WProgram.h"
#include "avr/interrupt.h"
#include "Motor/motor.h"
#include "UART/UART.h"

extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}

void blink()
{
	pinMode(13, OUTPUT);

	for(;;)
	{
	    digitalWrite(13,HIGH);
	    delay(500);
	    digitalWrite(13,LOW);
	    delay(500);
	}
}

int main()
{

    init();
    //blink();
	motorinit();
	UARTinit();

    byte input[4];
    byte output[4];
	output[0] = 65;
	output[1] = 66;
	output[2] = 67;
	output[3] = 68;

	for(;;)
	{
		if(UARTreceive(input,4))
		{
			updateRight((int8_t) input[0]);
			updateLeft((int8_t) input[1]);
		}

		delay(100);
		UARTsend(output,4);
	}

    for (;;) {}
    return 0;
}

