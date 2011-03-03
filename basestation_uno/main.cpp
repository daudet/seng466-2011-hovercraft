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
#include "wii_controller/WiiClassic.h"
#include "Wire/Wire.h"

uint8_t rx_addr[5] = { 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 };
uint8_t tx_addr[5] = { 0xE2, 0xE2, 0xE2, 0xE2, 0xE2 };

// this has to be volatile because it's used in an ISR (radio_rxhandler).
volatile uint8_t rxflag = 0;

int gamepadReceive(byte*, WiiClassic);

extern "C" void __cxa_pure_virtual()
{
    cli();
    for(;;);
}

radiopacket_t packet;

int main()
{

	sei();
	init();

	//instantiate the WiiClassic object
	WiiClassic wiiClassy = WiiClassic();

	Wire.begin();
	Serial.begin(9600);
	wiiClassy.begin();
	wiiClassy.update();

    // string buffer for printing to UART
    char output[128];

    //power on nrf24L01
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
    delay(50);

    // disable interrupts during setup
    cli();

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

	byte gamepadData[5];

	for(;;){

		gamepadReceive(gamepadData, wiiClassy);

		//fill the packet with data
		memcpy(packet.payload.message.messagecontent, gamepadData, 5);
		memcpy(packet.payload.message.address, rx_addr, 5);
		packet.payload.message.messageid = 1;

		//send the packet to the remote station; don't wait for successful transmission
		Radio_Transmit(&packet, RADIO_RETURN_ON_TX);
		
		/* to be implemented for debug from remote uno board
		if (rxflag)
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
/*
 * gamepadData is structured as follows:
 * gamepadData[0] = right analog joystick value from -127 --> 127
 * gamepadData[1] = left analog joystick value from -127 --> 127
 * gamepadData[2] = emergency stop button (right shoulder button) either 0 or 1
 * gamepadData[3] = lifting motor actuator (right Z button) either 0 or 1
 * gamepadData[4] = lifting motor actuator (left Z button) either 0 or 1
 */

int gamepadReceive(byte* gamepadData, WiiClassic wiiClassy){

	delay(1000);
	wiiClassy.update();

	//copy right analog data byte into message_content
	Serial.print("right stick:");
	gamepadData[0] = (byte)(map(wiiClassy.rightStickY(), 1, 28, -127, 127));
	Serial.println((int8_t)gamepadData[0]);

	//copy left analog data byte into message_content
	Serial.print("left stick:");
	gamepadData[1] = (byte)(map(wiiClassy.leftStickY(), 5, 59, -127, 127));
	Serial.println((int8_t)gamepadData[1]);

	Serial.print("right shoulder:");

	//copy emergency stop byte from button six
	if(wiiClassy.rightShoulderPressed()){
		gamepadData[2] = 1;
		Serial.println("1");
	}
	else{
		gamepadData[2] = 0;
		Serial.println("0");
	}

	Serial.print("right Z button:");

	//check for right Z button pressed

	if(wiiClassy.rzPressed()){
		gamepadData[3] = 1;
		Serial.println("1");
	}
	else{
		gamepadData[3] = 0;
		Serial.println("0");
	}

	Serial.print("left Z button:");

	//check for left Z button pressed
	if(wiiClassy.lzPressed()){
		gamepadData[4] = 1;
		Serial.println("1");
	}
	else{
		gamepadData[4] = 0;
		Serial.println("0");
	}

	return 1;
}
