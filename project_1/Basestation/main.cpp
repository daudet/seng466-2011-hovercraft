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

	//Serial.begin(38400);

	Mirf.csnPin = 7;
	Mirf.cePin = 8;
	Mirf.init();
	Mirf.setRADDR((byte *)"clie1");
	Mirf.payload = 18;
	Mirf.setTADDR((byte *)"serv1");
	Mirf.config();

	//MIRFinit(1);

	byte data[18];
	//uint8_t i = 0;

	for(;;){

		//initialize the data
		//for(i = 0; i < 18; i++)
			//data[i] = 0;

		initbuffer(data, 18);
		UARTreceive(data, 18);

		Mirf.send(data);

		while(Mirf.isSending());

	}
	for (;;);

	return 0;
}
