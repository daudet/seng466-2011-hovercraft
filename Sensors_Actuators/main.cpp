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
#include "Sonar/sonar.h"
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
	sonarinit();
	UARTinit();

    byte input[4];
    byte output[4];
	initbuffer(input,4);
	initbuffer(output,4);

	for(;;)
	{

		//receive motor commands
		UARTreceive(input,4);

		//drive motors
		updateRight((int8_t) input[0]);
		updateLeft((int8_t) input[1]);

		//read sonar
		output[0] = 1;
		output[1] = 2;

		//send sonar
		UARTsend(output,4);

	}

    for (;;) {}
    return 0;
}

