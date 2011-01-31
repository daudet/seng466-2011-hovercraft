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
void __cxa_pure_virtual(void){}

int main(void)
{
	init();
	UARTinit();
	//Serial.begin(9600);
	//Serial.println("Starting Server...");

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

	//Configure reciving address.
	Mirf.setRADDR((byte *)"serv1");

   // Set the payload length to sizeof(unsigned long) the
   // return type of millis().
   //NB: payload on client and server must be the same.

	Mirf.payload = sizeof(byte) * 4;

	//Write channel and payload config then power up reciver.
	Mirf.config();

	//Serial.println("Listening...");
	byte WIFIdata[Mirf.payload];
	initbuffer(WIFIdata, Mirf.payload);

	for(;;){


		Wire.beginTransmission(0x09);
		Wire.send('n');
		Wire.send(0x00);
		Wire.send(0x00);
		Wire.send(0x00);
		Wire.endTransmission();

		if(Mirf.dataReady()){
			do{
				//Serial.println("Got packet!");
				Mirf.getData(WIFIdata);

				//Serial.write(data, 4);

				if((int8_t)WIFIdata[0] < 0){
					//Serial.println("Got right joystick UP command...");

					Wire.beginTransmission(0x09);
					Wire.send('n');
					Wire.send(0x00);
					Wire.send(0xff);
					Wire.send(0x00);
					Wire.endTransmission();
				}
				if((int8_t)WIFIdata[1] < 0){
					//Serial.println("Got left joystick UP command...");

					Wire.beginTransmission(0x09);
					Wire.send('n');
					Wire.send(0x00);
					Wire.send(0x00);
					Wire.send(0xff);
					Wire.endTransmission();
				}

				UARTsend(WIFIdata, Mirf.payload);

				//Mirf.setTADDR((byte*)"clie1");
				//Mirf.send(data);
				//while(Mirf.isSending()){
					//delay(100);
				//}
				//Serial.println("Reply Sent...");

			}while(!Mirf.rxFifoEmpty());
		}
	}
	for (;;);

	return 0;
}
