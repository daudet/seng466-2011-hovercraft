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
#include "TTSched/scheduler.h"

//define identifiers for each tasks to be scheduled
typedef enum _tid{
	radioRX = 0,
	radioTX = 1,
	gamepadRX = 2
}TASK_IDS;

uint8_t rx_addr[5] = { 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 };
uint8_t tx_addr[5] = { 0xE2, 0xE2, 0xE2, 0xE2, 0xE2 };
uint8_t messageid = 0;

// this has to be volatile because it's used in an ISR (radio_rxhandler).
volatile uint8_t rxflag = 0;

int gamepad_receive(byte*, WiiClassic);
void radio_receive();
void radio_transmit();

extern "C" void __cxa_pure_virtual()
{
    cli();
    for(;;);
}

radiopacket_t packet;
byte gamepadData[5];

/*	this function does nothing, but could be
	used to accomplish something useful with any
	idle time
*/
void idle(uint32_t idle_period){
	//delay(20);
	//use idle time to receive debug pkts
	radio_receive();
}

/*
 * gamepadData is structured as follows:
 * gamepadData[0] = right analog joystick value from -127 --> 127
 * gamepadData[1] = left analog joystick value from -127 --> 127
 * gamepadData[2] = lifting motor actuator (right Z button) either 0 or 1
 * gamepadData[3] = lifting motor actuator (left Z button) either 0 or 1
 * gamepadData[4] = emergency stop button (right shoulder button) either 0 or 1
 */
int gamepad_receive(byte* gamepadData, WiiClassic wiiClassy){

	delay(50);
	wiiClassy.update();

	//copy right analog data byte into message_content
	//Serial.print("right stick:");
	gamepadData[0] = (byte)(constrain((map(wiiClassy.rightStickY(), 1, 28, -127, 127) - 4), -127, 128));
	//Serial.println((int8_t)gamepadData[0]);

	//copy left analog data byte into message_content
	//Serial.print("left stick:");
	gamepadData[1] = (byte)(constrain(map(wiiClassy.leftStickY(), 5, 59, -127, 127), -127, 127));
	//Serial.println((int8_t)gamepadData[1]);

	//Serial.print("right Z button:");

	//check for right Z button pressed

	if(wiiClassy.rzPressed()){
		gamepadData[2] = 1;
		//Serial.println("1");
	}
	else{
		gamepadData[2] = 0;
		//Serial.println("0");
	}

	//Serial.print("left Z button:");

	//check for left Z button pressed
	if(wiiClassy.lzPressed()){
		gamepadData[3] = 1;
		//Serial.println("1");
	}
	else{
		gamepadData[3] = 0;
		//Serial.println("0");
	}

	//Serial.print("right shoulder:");

	//copy emergency stop byte from button six
	if(wiiClassy.rightShoulderPressed()){
		gamepadData[4] = 1;
		//Serial.println("1");
	}
	else{
		gamepadData[4] = 0;
		//Serial.println("0");
	}

	return 1;
}

void radio_transmit(){
	Radio_Set_Tx_Addr(tx_addr);
	packet.type = MESSAGE;

	//instantiate the WiiClassic object
	WiiClassic wiiClassy = WiiClassic();

	Wire.begin();
	wiiClassy.begin();
	wiiClassy.update();

	gamepad_receive(gamepadData, wiiClassy);

	//fill the packet with data
	memcpy(packet.payload.message.messagecontent, gamepadData, 5);
	memcpy(packet.payload.message.address, rx_addr, 5);
	packet.payload.message.messageid = ++messageid;

	//send the packet to the remote station; don't wait for successful transmission
	Radio_Transmit(&packet, RADIO_RETURN_ON_TX);
	//Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);

}

void radio_receive(){
    char output[128];

	if (rxflag){
		Serial.println("Received a packet !!!");

		if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS){
			// if there are no more packets on the radio, clear the receive flag;
			// otherwise, we want to handle the next packet on the next loop iteration.
			Serial.println("No more packets to receive");
			rxflag = 0;
			Radio_Flush();
		}

		// This station is only expecting to receive DEBUG packets.
		if (packet.type == MESSAGE){
			snprintf(output, 128, "Successfully received MESSAGE from remote station\nmessage contents: %s", packet.payload.message.messagecontent);
			Serial.println(output);
		}

	}
}

/**
* This function is a hook into the radio's ISR.  It is called whenever the radio generates an RX_DR (received data ready) interrupt.
*/
void radio_rxhandler(uint8_t pipenumber){
    // just set a flag and toggle an LED.  The flag is polled in the main function.
	rxflag = 1;
}

int main(){

	sei();
	init();

	Serial.begin(9600);

    // string buffer for printing to UART
    char output[128];

    // print a message to UART to indicate that the program has started up
    snprintf(output, 128, "Starting Remote Station\n\r");
    Serial.print(output);

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

    //initialize the scheduler
    Scheduler_Init();

    //setup the tasks to be run
    Scheduler_StartTask(radioTX, 0, 75, radio_transmit);

    // enable interrupts
    sei();

    for(;;){
        	//run through the task list and run tasks as required
        	uint32_t idle_period = Scheduler_Dispatch();
        	    if (idle_period){
        	    	idle(idle_period);
        	    }
        }
        for(;;);
        return 0;
}
