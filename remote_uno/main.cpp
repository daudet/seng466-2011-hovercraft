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
#include "TTSched/scheduler.h"

//function prototypes
void radio_transmit();
void radio_receive();

//define identifiers for each tasks to be scheduled
typedef enum _tid{
	radioRX = 0,
	sonarRX = 1,
	green = 2,
	orange = 3,
	red = 4,
	radioTX = 5

}TASK_IDS;

uint8_t rx_addr[5] = { 0xE2, 0xE2, 0xE2, 0xE2, 0xE2 };
uint8_t tx_addr[5] = { 0xE1, 0xE1, 0xE1, 0xE1, 0xE1 };

// this has to be volatile because it's used in an ISR (radio_rxhandler).
volatile uint8_t rxflag = 0;
radiopacket_t packet;

//flags used to set state of FSM
uint8_t emergFlag = 0;
uint8_t autoFlag = 0;

// string buffer for printing to UART
char output[128];

unsigned long t1 = 0;

extern "C" void __cxa_pure_virtual()
{
    cli();
    for(;;);
}

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
	delay(20);

}

void blink_red(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0xff);
	Wire.send(0x00);
	Wire.send(0x00);
	Wire.endTransmission();
	//delay(20);
}

void blink_orange(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('n');
	Wire.send(0xff);
	Wire.send(0x66);
	Wire.send(0x00);
	Wire.endTransmission();
	//delay(20);
}
void blink_emergencyFlash(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('p');
	Wire.send(0x18);
	Wire.send(0x00);
	Wire.send(0x00);
	Wire.send('f');
	Wire.send(25);
	Wire.endTransmission();
}

void blink_stop(){
	Wire.beginTransmission(0x09);
	Wire.send('o');
	Wire.send('c');
	Wire.send(0x00);
	Wire.send(0x00);
	Wire.send(0x00);
	Wire.endTransmission();
}

/*	this function does nothing, but could be
	used to accomplish something useful with any
	idle time
*/
void idle(uint32_t idle_period){

	delay(idle_period);
	//radio_transmit();
}

void radio_receive(){
	char output[128];

	if (rxflag){
		if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
		{
			// if there are no more packets on the radio, clear the receive flag;
			// otherwise, we want to handle the next packet on the next loop iteration.
			rxflag = 0;
			Radio_Flush();
		}

		// This station is only expecting to receive MESSAGE packets.
		if (packet.type != MESSAGE)
		{
			//light up the LED
			blink_red();
			snprintf(output, 128, "Error: wrong packet type (type %d).\n\r", packet.type);
			Serial.print(output);
			return;
		}


		Serial.println(millis()-t1);
		//If emergency stop button pressed toggle emergFlag
		if((packet.payload.message.messagecontent[4] == 1) && ((millis() - t1) > 1000.0)){
			emergFlag = (emergFlag + 1) % 2;
			//keep track of time last toggle
			t1 = millis();
		}

		if(emergFlag){
			//stop motors
			//digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			//blink_emergencyFlash();
			blink_red();
			delay(20);
			blink_orange();
		}
		else{
			digitalWrite(5, HIGH);
			blink_stop();
		}
		Radio_Flush();
		Serial.flush();

		//If emergency stop button pressed toggle emergFlag
		if(packet.payload.message.messagecontent[3] == 1)
			autoFlag = (autoFlag + 1) % 2;

		if(autoFlag){
			//read sonar data and calculate values to send to MEGA
		}
		else{
			//pass on gamepad data to MEGA
			UARTsend(packet.payload.message.messagecontent, 4);
		}
	}
}

void radio_transmit(){

		Serial.println("Attempting to send debug message");
		//create debug packet
		Radio_Set_Tx_Addr(tx_addr);
		packet.type = MESSAGE;

	    snprintf(output, 128, "emergFlag=%d autoFlag=%d", emergFlag, autoFlag);
		memcpy(packet.payload.message.messagecontent, output, 23);

		if (Radio_Transmit(&packet, RADIO_RETURN_ON_TX) == RADIO_TX_MAX_RT){
			Serial.println("Could not send debug message to base station");
		}
		else{
			Serial.println("Successfully sent debug message to base station");
		}
}

//read the sonar data from UART
void sonar_receive(){

	byte data[4];
	int8_t sonar_diff;

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
}

int main()
{

    sei();
    init();

    //setup the BlinkM LED
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

    //initialize the scheduler
    Scheduler_Init();

    //setup the tasks to be run
    Scheduler_StartTask(radioRX, 0, 50, radio_receive);
    Scheduler_StartTask(radioTX, 0, 50, radio_transmit);
    //Scheduler_StartTask(red, 0, 800, blink_red);
    //Scheduler_StartTask(green, 0, 400, blink_green);
    //Scheduler_StartTask(orange, 0, 800, blink_orange);


    // enable interrupts
    sei();

    // print a message to UART to indicate that the program has started up
    snprintf(output, 128, "Starting Remote Station\n\r");
    Serial.print(output);


    for(;;){
    	//run through the task list and run tasks as required
    	uint32_t idle_period = Scheduler_Dispatch();

    	if (idle_period)
    	    idle(idle_period);
    }
    for(;;);
    return 0;
}

/**
* This function is a hook into the radio's ISR.  It is called whenever the radio generates an RX_DR (received data ready) interrupt.
*/
void radio_rxhandler(uint8_t pipenumber){
    // just set a flag and toggle an LED.  The flag is polled in the main function.
	rxflag = 1;
}
