/*
 * main.cpp
 *
 *  Created on: Jan 30, 2011
 *      Author: davidaudet
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */

#include "Core/WProgram.h"
#include "Spi/Spi.h"
#include "Mirf/Mirf.h"
#include "Mirf/nRF24L01.h"

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void){}

int main(void)
{
	init();

	Serial.begin(38400);
	Serial.println("Starting Client...");

	Mirf.csnPin = 7;
	Mirf.cePin = 8;

	Mirf.init();

	//Configure receiving address.
	Mirf.setRADDR((byte *)"clie1");

   // Set the payload length to sizeof(unsigned long) the
   // return type of millis().
   //NB: payload on client and server must be the same.

	Mirf.payload = sizeof(byte) * 18;

	//Write channel and payload config then power up reciver.
	Mirf.config();

	Serial.println("Starting to send...");

	for(;;){
		byte data[18];
		uint8_t i = 0;

		//initialize the data
		for(i = 0; i < 18; i++){
				data[i] = 0;
		}
		//wait until a full packet has been buffered
		while (Serial.available() < 18);

		//fill the buffer with the 20 bytes received
		for (i = 0; i < 18 ; i++)
			data[i] = Serial.read();

		//flush the serial buffer
		Serial.flush();

		Mirf.setTADDR((byte *)"serv1");

		Mirf.send(data);

		while(Mirf.isSending()){
		  }
		  Serial.println("Finished sending");
		  delay(10);
		  while(!Mirf.dataReady()){
		    Serial.println("Waiting");
		  }

		  //Mirf.getData(data);

		  //Serial.print("Received: ");
		  //Serial.println(data[15]);

		  //delay(1000);
	}
	for (;;);

	return 0;
}
