/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "Core/WProgram.h"
#include "avr/interrupt.h"
#define BUFFERSIZE 18

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
	Serial.begin(9600);

	// set PWM frequency to 32khz
	TCCR3B |=  _BV(CS30);
	TCCR3B &= ~_BV(CS32);
	TCCR3B &= ~_BV(CS31);

	//set pwm outputs
	pinMode(E3-4, OUTPUT); //E3-4 right
	pinMode(E1-2, OUTPUT); //E1-2 left

	//Motor directions
	pinMode(8, OUTPUT); //L3 right
	pinMode(9, OUTPUT); //L4 right
	pinMode(10, OUTPUT); //L1 left
	pinMode(11, OUTPUT); //L2 left

	//default values (off)
	digitalWrite(8, LOW);
	digitalWrite(9, HIGH);
	digitalWrite(10, LOW);
	digitalWrite(11, HIGH);
	digitalWrite(2, LOW);
	digitalWrite(3,LOW);

	//counter
	uint8_t i = 0;

	//input buffer
	int8_t input[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++){input[i] = 0;}

	//output buffer
	int8_t output[BUFFERSIZE];
	for(i = 0; i < BUFFERSIZE; i++){output[i] = 0;}

	for(;;){
		// wait until a full packet has been buffered
		while (Serial.available() < BUFFERSIZE);

		//fill the buffer with the bytes received
		for (i = 0; i < 18 ; i++)
			data[i] = Serial.read();
		Serial.flush();

		//RIGHTSIDE
		if(data[15] < 0){ //UP
			digitalWrite(8, LOW);
			digitalWrite(9, HIGH);
			analogWrite(2, abs(data[15])<<1);
			digitalWrite(13, HIGH); //debug
		}
		else if(data[15] > 0){ //DOWN
			digitalWrite(8, HIGH);
			digitalWrite(9, LOW);
			analogWrite(2, abs(data[15])<<1);
			digitalWrite(13, LOW); //debug
		}
		else{
			digitalWrite(2, LOW);
			digitalWrite(13, LOW); //debug
		}

		//LEFTSIDE
		if(data[13] < 0){//UP
			digitalWrite(10, LOW);
			digitalWrite(11, HIGH);
			analogWrite(3, abs(data[13])<<1);
		}
		else if(data[13] > 0){//DOWN
			digitalWrite(10, HIGH);
			digitalWrite(11, LOW);
			analogWrite(3, abs(data[13])<<1);
		}
		else{
			digitalWrite(3, LOW);
		}
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
