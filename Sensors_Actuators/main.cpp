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

int main()
{

    init();
    //blink();
	motorinit();
	sonarinit();
	UARTinit();

    byte input[BUFFERSIZE];
    byte output[BUFFERSIZE];
	UARTinitbuffer(input);
	UARTinitbuffer(output);

	for(;;){

		//receive motor commands
		UARTreceive(input);

		//drive motors
		updateRight((int8_t) input[13]);
		updateLeft((int8_t) input[15]);

		//read sonar
		output[0] = 1;
		output[1] = 2;

		//send sonar
		UARTsend(output);



	}

    for (;;) {}
    return 0;
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
