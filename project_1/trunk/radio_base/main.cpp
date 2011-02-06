/*
 * **************** RADIO_BASE *********************
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
#include "UART/UART.h"

uint8_t rx_addr[5] = { 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 };
uint8_t tx_addr[5] = { 0xE2, 0xE2, 0xE2, 0xE2, 0xE2 };

// this has to be volatile because it's used in an ISR (radio_rxhandler).
volatile uint8_t rxflag = 0;

extern "C" void __cxa_pure_virtual()
{
    cli();
    for(;;);
}

radiopacket_t packet;

int main()
{
    // string buffer for printing to UART
    //char output[128];
    sei();
    init();

    //power on nrf24L01
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
    delay(50);

    // disable interrupts during setup
    cli();

    // start the serial output module
    UARTinit();

    // initialize the radio, including the SPI module
    Radio_Init();

    // configure the receive settings for radio pipe 0
    Radio_Configure_Rx(RADIO_PIPE_0, rx_addr, ENABLE);
    // configure radio transceiver settings.
    Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

    // enable interrupts
    sei();

	Radio_Set_Tx_Addr(tx_addr);
	packet.type = MESSAGE;

	byte data[18];
	byte message_content[4];

	for(;;){

		//get the gamepad data
		if(UARTreceive(data, 18)){

			//copy right analog data byte into message_content
			message_content[0] = data[15];

			//copy left analog data byte into message_content
			message_content[1] = data[13];

			//fill the packet with data
			memcpy(packet.payload.message.messagecontent, message_content, 4);
			memcpy(packet.payload.message.address, rx_addr, 5);
			packet.payload.message.messageid = 1;

			//send the packet to the remote station; don't wait for successful transmission
			Radio_Transmit(&packet, RADIO_RETURN_ON_TX);

		}

		/*if (rxflag)
		{
				//Serial.println("Received a packet !!!");

			if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
			{
				// if there are no more packets on the radio, clear the receive flag;
				// otherwise, we want to handle the next packet on the next loop iteration.
				//Serial.println("No more packets to receive");
				rxflag = 0;
			}

			// This station is only expecting to receive MESSAGE packets.
			if (packet.type == ACK)
			{
				//snprintf(output, 128, "Successfully got ACK from DEBUG for message id: %d \n\r", packet.payload.ack.messageid);
				//Serial.print(output);

				//copy message into the packet
				memcpy(packet.payload.message.messagecontent, data, 18);
				memcpy(packet.payload.message.address, rx_addr, 5);

				//send the next packet
				packet.type = MESSAGE;
				packet.payload.message.messageid = packet.payload.message.messageid + 1;

				if(Radio_Transmit(&packet, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT)
				{
					//snprintf(output, 128, "Could not send message to debug.\n\r");
					//Serial.print(output);
				}
				else
				{
					//snprintf(output, 128, "Successfully sent message to debug.\n\r");
					//Serial.print(output);
				}
			}
		}*/
	}
    return 0;
}

/**
* This function is a hook into the radio's ISR.  It is called whenever the radio generates an RX_DR (received data ready) interrupt.
*/
void radio_rxhandler(uint8_t pipenumber)
{
    // just set a flag and toggle an LED.  The flag is polled in the main function.
	//rxflag = 1;
}

