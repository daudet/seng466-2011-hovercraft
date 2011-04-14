/*
 * motor.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: cei
 */

#include "motor.h"

void motorinit()
{
	// set PWM frequency to 32khz (lift motors)
	TCCR3B |=  _BV(CS30);
	TCCR3B &= ~_BV(CS32);
	TCCR3B &= ~_BV(CS31);
	//movement motors
	TCCR4B |=  _BV(CS40);
	TCCR4B &= ~_BV(CS42);
	TCCR4B &= ~_BV(CS41);

	//set pwm outputs
	pinMode(rightE, OUTPUT); //right
	pinMode(leftE, OUTPUT); //left
	pinMode(strafeE, OUTPUT); //strafe

	//Motor directions
	pinMode(right1, OUTPUT);
	pinMode(right2, OUTPUT);
	pinMode(left1, OUTPUT);
	pinMode(left2, OUTPUT);
	pinMode(strafe1, OUTPUT);
	pinMode(strafe2, OUTPUT);

	//default values (off)
	digitalWrite(right1, LOW);
	digitalWrite(right2, HIGH);
	digitalWrite(left1, LOW);
	digitalWrite(left2, HIGH);
	digitalWrite(strafe1, LOW);
	digitalWrite(strafe2, HIGH);
	digitalWrite(rightE, LOW);
	digitalWrite(leftE, LOW);
	digitalWrite(strafeE, LOW);


	// lifting motors
	pinMode(backE,OUTPUT);
	pinMode(frontE,OUTPUT);

	//default to 'off'!
	analogWrite(backE, 0);
	analogWrite(frontE, 0);

}

//value is between -127 and 127.
void updateRight(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(right1, LOW);
		digitalWrite(right2, HIGH);
		analogWrite(rightE, map(abs(value),1,127,DEADZONE,255));
	}
	else if(value > 0){ //DOWN
		digitalWrite(right1, HIGH);
		digitalWrite(right2, LOW);
		analogWrite(rightE, map(abs(value),1,127,DEADZONE,255));
	}
	else{
		digitalWrite(rightE, LOW);
	}
}

//value is between -127 and 127.
void updateLeft(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(left1, LOW);
		digitalWrite(left2, HIGH);
		analogWrite(leftE,  map(abs(value),1,127,DEADZONE,255));
	}
	else if(value > 0){ //DOWN
		digitalWrite(left1, HIGH);
		digitalWrite(left2, LOW);
		analogWrite(leftE,  map(abs(value),1,127,DEADZONE,255));
	}
	else{
		digitalWrite(leftE, LOW);
	}
}

//value is between -127 and 127.
void updateStrafe(int8_t value)
{
	if(value < 0){ //UP
		digitalWrite(strafe1, LOW);
		digitalWrite(strafe2, HIGH);
		analogWrite(strafeE,  map(abs(value),1,127,DEADZONE,255));
	}
	else if(value > 0){ //DOWN
		digitalWrite(strafe1, HIGH);
		digitalWrite(strafe2, LOW);
		analogWrite(strafeE,  map(abs(value),1,127,DEADZONE,255));
	}
	else{
		digitalWrite(strafeE, LOW);
	}
}
