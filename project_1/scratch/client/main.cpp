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
#include "UART/UART.h"

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void){}

int main(void)
{
	init();

	UARTinit();
	//Serial.println("Starting Client...");

	Mirf.csnPin = 7;
	Mirf.cePin = 8;

	Mirf.init();

	//Configure receiving address.
	Mirf.setRADDR((byte *)"clie1");

   // Set the payload length to sizeof(unsigned long) the
   // return type of millis().
   //NB: payload on client and server must be the same.

	Mirf.payload = sizeof(byte) * 4;

	//Write channel and payload config then power up reciver.
	Mirf.config();

	//Serial.println("Starting to send...");
	byte WIFIdata[Mirf.payload];
	byte UARTdata[Mirf.payload];
	byte GAMEdata[18];

	initbuffer(UARTdata, 4);
	initbuffer(WIFIdata, 4);
	initbuffer(GAMEdata, 18);

	for(;;){

		UARTreceive(GAMEdata,18);

		WIFIdata[0]=GAMEdata[13];
		WIFIdata[1]=GAMEdata[15];

		Mirf.setTADDR((byte *)"serv1");

		Mirf.send(WIFIdata);

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
