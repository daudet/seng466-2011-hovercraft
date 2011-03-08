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
#include "radio.h"
#include "Wire/Wire.h"
#include "wiring.h"
#include "UART/UART.h"

uint8_t rx_addr[5] = { 0xE2, 0xE2, 0xE2, 0xE2, 0xE2 };
uint8_t tx_addr[5] = { 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 };

// this has to be volatile because it's used in an ISR (radio_rxhandler).
volatile uint8_t rxflag = 0;

extern "C" void __cxa_pure_virtual()
{
    cli();
    for(;;);
}

radiopacket_t packet;

void blinkm()
{
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	digitalWrite(A2, LOW);
	digitalWrite(A3, HIGH);
	delay(100);
	Wire.begin();

	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0xff);
	Wire.send(0x66);
	Wire.send(0x00);
	Wire.endTransmission();
	return;
}

void blink_green(){

	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0x00);
	Wire.send(0xff);
	Wire.send(0x00);
	Wire.endTransmission();

}

void blink_red(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0xff);
	Wire.send(0x00);
	Wire.send(0x00);
	Wire.endTransmission();
}

void blink_orange(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0xff);
	Wire.send(0x66);
	Wire.send(0x00);
	Wire.endTransmission();
}

int main()
{
    // string buffer for printing to UART
    char output[128];
    sei();
    init();

    blinkm();

    //output used for transistor
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    //power on nrf24L01
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
    delay(50);

    // disable interrupts during setup
    cli();

    // start the serial output module
    Serial.begin(9600);

    // initialize the radio, including the SPI module
    Radio_Init();

    // configure the receive settings for radio pipe 0
    Radio_Configure_Rx(RADIO_PIPE_0, rx_addr, ENABLE);
    // configure radio transceiver settings.
    Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

    // enable interrupts
    sei();

    // print a message to UART to indicate that the program has started up
    snprintf(output, 128, "Starting Remote Station\n\r");
    //Serial.print(output);
    byte data[4];
    int8_t sonar_diff;

	for (;;)
	    {
			if(UARTreceive(data, 4)){
				sonar_diff = (int8_t)data[0] - (int8_t)data [1];

				if(sonar_diff < 0){
					blink_red();
				}
				else if(sonar_diff == 0){
					blink_orange();
				}
				else
					blink_green();
			}

	        if (rxflag)
	        {
	            if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
	            {
	                // if there are no more packets on the radio, clear the receive flag;
	                // otherwise, we want to handle the next packet on the next loop iteration.
	                rxflag = 0;
	            }

	            // This station is only expecting to receive MESSAGE packets.
	            if (packet.type != MESSAGE)
	            {
	            	//light up the LED
	            	blink_red();
	                snprintf(output, 128, "Error: wrong packet type (type %d).\n\r", packet.type);
	                Serial.print(output);
	                continue;
	            }

	            // Set the transmit address to the one specified in the message packet.
	            //Radio_Set_Tx_Addr(packet.payload.message.address);

	            // Print out the message, along with the message ID and sender address.
	            snprintf(output, 128, "Message ID %d from 0x%.2X%.2X%.2X%.2X%.2X INFO-> right: %.2X left: %.2X emergency: %.2X\n\r",
	                    packet.payload.message.messageid,
	                    packet.payload.message.address[0],
	                    packet.payload.message.address[1],
	                    packet.payload.message.address[2],
	                    packet.payload.message.address[3],
	                    packet.payload.message.address[4],
	                    packet.payload.message.messagecontent[0],
	                    packet.payload.message.messagecontent[1],
	                    packet.payload.message.messagecontent[2]);
	            //Serial.print(output);

	            UARTsend(packet.payload.message.messagecontent, 4);

	            /* Emergency Stop */
	            if(packet.payload.message.messagecontent[4] == 1){

	            	//stop motors
	            	digitalWrite(4, LOW);

	            	for(;;){

	            		digitalWrite(5, LOW);
	            		blink_orange();
	            		delay(100);
	            		blink_red();
	            		delay(100);
	            		Radio_Flush();
	            		Serial.flush();
	            	}
	            }
	            /*
	            //create ACK packet
	            Radio_Set_Tx_Addr(tx_addr);
	            packet.type = ACK;

	            //send an ACK packet to base station
	            //Radio_Transmit(&packet, RADIO_RETURN_ON_TX);
	            Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
				*/
	        }
	    }
    return 0;
}

/**
* This function is a hook into the radio's ISR.  It is called whenever the radio generates an RX_DR (received data ready) interrupt.
*/
void radio_rxhandler(uint8_t pipenumber){
    // just set a flag and toggle an LED.  The flag is polled in the main function.
	rxflag = 1;
}

