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
#define BUFFERSIZE 18
#define BAUD 38400

extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}

int main()
{
    init();
    //blink();

    //Open serial
	Serial.begin(BAUD);

	//counter
	uint8_t i = 0;

	//input buffer
	int8_t input[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++)
		input[i] = 0;

	//output buffer
	int8_t output[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++)
		output[i] = 0;

	//initialize motor
	motorinit();

	for(;;){
		// wait until a full packet has been buffered
		while (Serial.available() < BUFFERSIZE);

		//fill the buffer with the bytes received
		for (i = 0; i < BUFFERSIZE ; i++)
			input[i] = Serial.read();
		Serial.flush();

		//RIGHTSIDE
		updateRight(input[13]);
		updateLeft(input[15]);

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
