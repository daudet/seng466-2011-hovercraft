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
	Serial.begin(38400);
	uint8_t data[20];
	uint8_t i = 0;

	pinMode(13, OUTPUT);

	for(;;){
		// wait until a full packet has been buffered
		delay(100);
		//while (Serial.available() < 20);


		//fill the buffer with the 20 bytes received
		for (i = 0; i < 20; i++)
			data[i] = Serial.read();

		//if button one is pressed blink LED pin 13
		if(data[0] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[1] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[2] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}
		if(data[3] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[4] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[5] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[6] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}
		if(data[7] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}
		if(data[8] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[9] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[10] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}
		if(data[11] == 1){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13,LOW);
		}

		if(data[13] == 99){
			digitalWrite(13,HIGH);
			delay(10);
			digitalWrite(13, LOW);
		}
	}

	//ensure that main never returns and runs arbitrary code
	for(;;);
	return 0;
}
