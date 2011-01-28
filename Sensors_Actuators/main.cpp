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
	motorinit();
	sonarinit();


    //Open serial
	Serial.begin(BAUD);

	//counter
	uint8_t i = 0;

	//input buffer
	int8_t input[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++)
		input[i] = 0;

	//output buffer
	byte output[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++)
		output[i] = 0;

	for(;;){

		// wait until a full packet has been buffered
		while (Serial.available() < BUFFERSIZE);

		//fill the buffer with the bytes received
		for (i = 0; i < BUFFERSIZE ; i++)
			input[i] = Serial.read();
		Serial.flush();

		//drive motors
		updateRight(input[13]);
		updateLeft(input[15]);

		//read sonar
		output[0]=1;
		output[1]=2;

		//send sonar
		Serial.write(output,BUFFERSIZE);

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
