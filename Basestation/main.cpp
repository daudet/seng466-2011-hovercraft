/**
     * An Mirf example which copies back the data it recives.
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

#include "Spi/Spi.h"
#include "Mirf/Mirf.h"
#include "Mirf/nRF24L01.h"
#include "Core/Wprogram.h"
#include "avr/interrupt.h"

extern "C" void __cxa_pure_virtual(void){
	for(;;);
}

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

    Mirf.setRADDR((byte *)"serv1");

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

    Mirf.config();

    Serial.println("Listening...");
}

void loop(){
      /*
       * A buffer to store the data.
       */

      byte data[Mirf.payload];

      /*
       * If a packet has been recived.
       */
      if(Mirf.dataReady()){

        do{
          Serial.println("Got packet");

          /*
           * Get load the packet into the buffer.
           */

          Mirf.getData(data);

          /*
           * Set the send address.
           */


          Mirf.setTADDR((byte *)"clie1");

          /*
           * Send the data back to the client.
           */

          Mirf.send(data);

          /*
           * Wait untill sending has finished
           *
           * NB: isSending returns the chip to receving after returning true.
           */

          while(Mirf.isSending()){
            delay(100);
          }

          Serial.println("Reply sent.");
        }while(!Mirf.rxFifoEmpty());
    }
}

//int main(){
//
//	init();
//
//	//TCCR3B |= _BV(CS30);
//	//TCCR3B &= ~_BV(CS32);
//	//TCCR3B &= ~_BV(CS31);
//
//	pinMode(2, OUTPUT);
//	pinMode(3, OUTPUT);
//
//	//debug
//	pinMode(13, OUTPUT);
//
//	//Motor directions
//	pinMode(8, OUTPUT);
//	pinMode(9, OUTPUT);
//	pinMode(10, OUTPUT);
//	pinMode(11, OUTPUT);
//
//	digitalWrite(8, LOW);
//	digitalWrite(9, HIGH);
//
//	digitalWrite(10, LOW);
//	digitalWrite(11, HIGH);
//
//	digitalWrite(2, LOW);
//	digitalWrite(3,LOW);
//
//	Serial.begin(38400);
//	int8_t data[18];
//
//
//	uint8_t i = 0;
//	for(i = 0; i < 18; i++){
//		data[i] = 0;
//	}
//
//
//
//	for(;;){
//		// wait until a full packet has been buffered
//		while (Serial.available() < 18);
//
//		//fill the buffer with the 20 bytes received
//		for (i = 0; i < 18 ; i++)
//			data[i] = Serial.read();
//			Serial.flush();
//
//			//RIGHTSIDE
//			if(data[15] < 0){ //UP
//				digitalWrite(8, LOW);
//				digitalWrite(9, HIGH);
//				analogWrite(2, abs(data[15])<<1);
//				digitalWrite(13, HIGH); //debug
//			}
//			else if(data[15] > 0){ //DOWN
//				digitalWrite(8, HIGH);
//				digitalWrite(9, LOW);
//				analogWrite(2, abs(data[15])<<1);
//				digitalWrite(13, LOW); //debug
//			}
//			else{
//				digitalWrite(2, LOW);
//				digitalWrite(13, LOW); //debug
//			}
//
//			//LEFTSIDE
//			if(data[13] < 0){//UP
//				digitalWrite(10, LOW);
//				digitalWrite(11, HIGH);
//				analogWrite(3, abs(data[13])<<1);
//			}
//			else if(data[13] > 0){//DOWN
//				digitalWrite(10, HIGH);
//				digitalWrite(11, LOW);
//				analogWrite(3, abs(data[13])<<1);
//			}
//			else{
//				digitalWrite(3, LOW);
//			}
//		}
//
//	//setup();
//
//	for (;;) {
//    	//loop();
//    }
//    return 0;
//}

int main()
{
    init();
    pinMode(13, OUTPUT);
    pinMode(11, OUTPUT);
    for(;;){
    	digitalWrite(13,HIGH);
    	delay(1000);
    	digitalWrite(13,LOW);
    	delay(1000);
    }
    for (;;) {}
    return 0;
}
