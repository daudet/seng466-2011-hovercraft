/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "Core/WProgram.h"
#include "avr/interrupt.h"
#include "Spi/Spi.h"
#include "Mirf/Mirf.h"
#include "Mirf/nRF24L01.h"

extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}

int main()
{
    init();
    setup();
    //pinMode(13, OUTPUT);
    //for(;;){
    //	digitalWrite(13,HIGH);
    //	delay(1000);
    //	digitalWrite(13,LOW);
    //	delay(1000);
    //}
    for (;;) {loop();}
    return 0;
}


/**
 * A Mirf example to test the latency between two Ardunio.
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
 * This depends on the Spi Library:
 * http://www.arduino.cc/playground/Code/Spi
 */



void setup(){
  Serial.begin(9600);
  /*
   * Setup pins / SPI.
   */


   Mirf.csnPin = 9;
   Mirf.cePin = 7;


  Mirf.init();

  /*
   * Configure reciving address.
   */

  Mirf.setRADDR((byte *)"clie1");

  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */

  Mirf.payload = sizeof(unsigned long);

  /*
   * Write channel and payload config then power up reciver.
   */

  /*
   * To change channel:
   *
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */

  Mirf.config();

  Serial.println("Beginning ... ");
}

void loop(){
  unsigned long time = millis();

  Mirf.setTADDR((byte *)"serv1");

  Mirf.send((byte *)&time);

  while(Mirf.isSending()){
  }
  Serial.println("Finished sending");
  delay(10);
  while(!Mirf.dataReady()){
    //Serial.println("Waiting");
  }

  Mirf.getData((byte *) &time);

  Serial.print("Ping: ");
  Serial.println((millis() - time));

  delay(1000);
}



