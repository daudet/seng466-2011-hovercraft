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

	//lift motors
	TCCR4B |=  _BV(CS40);
	TCCR4B &= ~_BV(CS42);
	TCCR4B &= ~_BV(CS41);

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


	// lifting motors
	pinMode(LE1,OUTPUT);
	pinMode(LE2,OUTPUT);
	//default to 'off'!
	analogWrite(LE1, 255);
	analogWrite(LE2, 255);

}

//value is between -100 and 99.
void updateRight(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(L3, LOW);
		digitalWrite(L4, HIGH);
		analogWrite(E3_4, map(abs(value),1,128,DEADZONE,255));
		digitalWrite(13,HIGH);
	}
	else if(value > 0){ //DOWN
		digitalWrite(L3, HIGH);
		digitalWrite(L4, LOW);
		analogWrite(E3_4, map(abs(value),1,128,DEADZONE,255));
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
		analogWrite(E1_2,  map(abs(value),1,128,DEADZONE,255));
		digitalWrite(13,HIGH);
	}
	else if(value > 0){ //DOWN
		digitalWrite(L1, HIGH);
		digitalWrite(L2, LOW);
		analogWrite(E1_2,  map(abs(value),1,128,DEADZONE,255));
		digitalWrite(13,LOW);
	}
	else{
		digitalWrite(E1_2, LOW);
		digitalWrite(13,LOW);
	}
}
