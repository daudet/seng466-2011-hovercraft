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
#include "MIRF/Mirf2.h"

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void){}

int main(void)
{
	init();

	UARTinit();

	MIRFinit(1);

	byte WIFIdata[4];
	byte UARTdata[4];
	byte GAMEdata[18];

	initbuffer(UARTdata, 4);
	initbuffer(WIFIdata, 4);
	initbuffer(GAMEdata, 18);

	for(;;){

		UARTreceive(GAMEdata,18);
		WIFIdata[0]=GAMEdata[13];
		WIFIdata[1]=GAMEdata[15];

		MIRFsend(WIFIdata);

	}
	for (;;);

	return 0;
}
