/*
 * main.cpp
 *
 *  Created on: Jan 23, 2011
 *      Author: daudet
 */

#include "include/WProgram.h"
#include "include/wiring.h"


extern "C" void __cxa_pure_virtual(){
	for(;;);
}

int main(void){

	init();

	TCCR3B |= _BV(CS30);
	TCCR3B &= ~_BV(CS32);
	TCCR3B &= ~_BV(CS31);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	//Motor directions
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);

	digitalWrite(8, LOW);
	digitalWrite(9, HIGH);

	digitalWrite(10, LOW);
	digitalWrite(11, HIGH);

	digitalWrite(2, LOW);
	digitalWrite(3,LOW);

	Serial.begin(38400);
	int8_t data[18];


	uint8_t i = 0;
	for(i = 0; i < 18; i++){
		data[i] = 0;
	}

	pinMode(13, OUTPUT);

	for(;;){
		// wait until a full packet has been buffered
		while (Serial.available() < 18);

		//fill the buffer with the 20 bytes received
		for (i = 0; i < 18 ; i++)
			data[i] = Serial.read();
		Serial.flush();

		//RIGHTSIDE
		if(data[15] < 0){ //UP
			digitalWrite(8, LOW);
			digitalWrite(9, HIGH);
			analogWrite(2, abs(data[15])<<1);
		}
		else if(data[15] > 0){ //DOWN
			digitalWrite(8, HIGH);
			digitalWrite(9, LOW);
			analogWrite(2, abs(data[15])<<1);
		}
		else{
			digitalWrite(2, LOW);
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

	//ensure that main never returns and runs arbitrary code
	for(;;);
	return 0;
}
