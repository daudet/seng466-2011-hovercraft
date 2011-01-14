/*
 * main.cpp
 *
 *  Created on: Jan 13, 2011
 *      Author: davidaudet
 */

#include <WProgram.h>

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void){}

int main(void)
{
	init();

	Serial.begin(9600);
	Serial.println("Starting routine blink_test...\n");

	for(;;){

		digitalWrite(13,1);
		delay(1000);
		digitalWrite(13,0);
		delay(1000);

//		int sensorValue = analogRead(A0);
//		Serial.println(sensorValue, DEC);
//		digitalWrite(13, 1);
//		delay(10);
//		digitalWrite(13, 0);
//		delay(sensorValue);
//		digitalWrite(13,1);

	}

	for (;;)

	return 0;
}
