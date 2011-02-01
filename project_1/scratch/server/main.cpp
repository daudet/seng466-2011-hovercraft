/*
 * BASESTATION SERVER
 *	This basestation will send data gathered from the serial
 *	port to the remote station via radio signal and
 *	cause the ATMega to adjust the dc motor accordingly
 *
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
#include "Core/wiring.h"
#include "Wire/Wire.h"
#include "UART/UART.h"

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void){
	cli();
	for(;;);
}

int main(void)
{
	init();
	UARTinit();

	//setup analog pins to be used as output
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	//set A2 to GND
	digitalWrite(A2, LOW);
	//set A3 to +5v
	digitalWrite(A3, HIGH);

	Wire.begin();

	Mirf.csnPin = 7;
	Mirf.cePin = 8;
	Mirf.init();
	Mirf.setRADDR((byte *)"serv1");
	Mirf.setTADDR((byte*)"clie1");
	Mirf.payload = 18;
	Mirf.config();

	byte data[Mirf.payload];

	for(;;){
		Serial.flush();

		initbuffer(data, 18);

		Wire.beginTransmission(0x09);
		Wire.send('n');
		Wire.send(0x00);
		Wire.send(0x00);
		Wire.send(0x00);
		Wire.endTransmission();

		if(Mirf.dataReady()){
			//do{
				Mirf.getData(data);

				if((int8_t)data[15] < 0){

					Wire.beginTransmission(0x09);
					Wire.send('n');
					Wire.send(0x00);
					Wire.send(0xff);
					Wire.send(0x00);
					Wire.endTransmission();
				}
				if((int8_t)data[13] < 0){

					Wire.beginTransmission(0x09);
					Wire.send('n');
					Wire.send(0x00);
					Wire.send(0x00);
					Wire.send(0xff);
					Wire.endTransmission();
				}

			//}
				while(!Mirf.rxFifoEmpty());
		}
	}
	for (;;);

	return 0;
}
