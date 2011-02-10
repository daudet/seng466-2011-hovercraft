/*
 * motor.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: cei
 */

#include "motor.h"

void motorinit()
{
	// set PWM frequency to 32khz
	TCCR3B |=  _BV(CS30);
	TCCR3B &= ~_BV(CS32);
	TCCR3B &= ~_BV(CS31);

	//set pwm outputs
	pinMode(E3_4, OUTPUT); //E3_4 right
	pinMode(E1_2, OUTPUT); //E1_2 left

	//Motor directions
	pinMode(L3, OUTPUT); //L3 right
	pinMode(L4, OUTPUT); //L4 right
	pinMode(L1, OUTPUT); //L1 left
	pinMode(L2, OUTPUT); //L2 left

	//default values (off)
	digitalWrite(L3, LOW);
	digitalWrite(L4, HIGH);
	digitalWrite(L1, LOW);
	digitalWrite(L2, HIGH);
	digitalWrite(E3_4, LOW);
	digitalWrite(E1_2, LOW);

	//debug
	pinMode(13,OUTPUT);
}

//value is between -100 and 99.
void updateRight(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(L3, LOW);
		digitalWrite(L4, HIGH);
		analogWrite(E3_4, abs(value)<<1);
		digitalWrite(13,HIGH);
	}
	else if(value > 0){ //DOWN
		digitalWrite(L3, HIGH);
		digitalWrite(L4, LOW);
		analogWrite(E3_4, abs(value)<<1);
		digitalWrite(13,LOW);
	}
	else{
		digitalWrite(E3_4, LOW);
		digitalWrite(13,LOW);
	}
}

//value is between -100 and 99.
void updateLeft(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(L1, LOW);
		digitalWrite(L2, HIGH);
		analogWrite(E1_2, abs(value)<<1);
		digitalWrite(13,HIGH);
	}
	else if(value > 0){ //DOWN
		digitalWrite(L1, HIGH);
		digitalWrite(L2, LOW);
		analogWrite(E1_2, abs(value)<<1);
		digitalWrite(13,LOW);
	}
	else{
		digitalWrite(E1_2, LOW);
		digitalWrite(13,LOW);
	}
}
