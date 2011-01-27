/*
 * controlroomba.cpp
 *
 *  Created on: 2010-02-01
 *      Author: River Allen
 */

#define ECLIPSE_BUILD		// includes the code required for Eclipse build (but not for Arduino build)
#ifdef ECLIPSE_BUILD

// This code is required if the project is being built under Eclipse
#include "WProgram.h"

//#include any additional .pde files which are part of the source here eg
//#include "F002.pde"

// Declare functions (including those used in additional .pde source files)
//eg double funcDec(char arg1, char arg2[], int arg3);
void setup();
void loop();

#endif

#include <string.h>
#include <stdio.h>
#include "common.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "radio/Nrf2401.h"

#define USB_JOYSTICK

#ifdef USB_JOYSTICK
#include "radio/packet.h"
#else
#include "adc/adc.h"
#endif


// Radio
#define RECEIVE_ADDRESS 0x0650
#define SEND_ADDRESS    0x0700
#define BUFLEN			64


Nrf2401 Radio;
int ledG1Pin =  29;
int ledR1Pin =  28;
int ledG2Pin =  30;
int ledR2Pin =  31;
uint8_t buf[BUFLEN];

#ifdef USB_JOYSTICK
pf_usb_joystick_data_t usb_data;
radiopacket_t send_packet;

void writeUART( char*, int );
#else
uint8_t send_packet[4];
// Misc
#define VACUUM_COUNT	1
#define TURBO_COUNT		10
uint8_t trigger_still_held = 0;
uint8_t trigger_hold_count = 0;

uint8_t joystick_trigger = 40;
uint8_t joystick_x = 45;
uint8_t joystick_y = 42;
uint8_t joystick_z = 43;
#endif




void setup()
{
	pinMode(ledG1Pin, OUTPUT);
	pinMode(ledR1Pin, OUTPUT);
	pinMode(ledR2Pin, OUTPUT);
	pinMode(ledR2Pin, OUTPUT);
	digitalWrite(ledG1Pin, HIGH);

	cli();
	clock8MHz();
	Radio.remoteAddress = SEND_ADDRESS;
	Radio.txMode(PAYLOAD_BYTES);
	beginSerial(38400);

#ifdef USB_JOYSTICK
	// pass
#else
	pinMode(joystick_trigger, INPUT);
	pinMode(joystick_x, INPUT);
	pinMode(joystick_y, INPUT);
	pinMode(joystick_z, INPUT);
	digitalWrite(joystick_x, LOW);
	digitalWrite(joystick_y, LOW);
	digitalWrite(joystick_z, LOW);
	ADC_Init();
#endif
	sei();

	writeUART("Initializing...", 15);
}

void writeUART( char* buffer, int len )
{
	int i = 0;

	while( i < len )
	{
		serialWrite( buffer[i] );
		++i;
	}
	serialWrite( '\r' );
	serialWrite( '\n' );
}

void readUART( uint8_t* buffer, int len)
{
	int i = 0;
	while (i < len)
	{
		buffer[i] = serialRead();
		++i;
	}
}

void loop()
{
#ifdef USB_JOYSTICK

	if (serialAvailable() >= sizeof(pf_usb_joystick_data_t))
	{
		// Ensure data integrity
		// Read

		readUART(buf, sizeof(pf_usb_joystick_data_t));

		//writeUART((char *)buf, sizeof(pf_usb_joystick_data_t));
		memcpy(&usb_data, buf, sizeof(pf_usb_joystick_data_t));
		send_packet.type = USB_JOYSTICK_DATA;
		send_packet.payload.joystick = usb_data;
		memcpy(buf, &send_packet, sizeof(radiopacket_t));

		// Send to roomba
		Radio.write(buf);
		digitalWrite(ledR2Pin, !digitalRead(ledR2Pin));
	}
#else
	uint8_t x = ADC_Sample(AD5);
	uint8_t y = ADC_Sample(AD2);
	uint8_t z = ADC_Sample(AD3);
	uint8_t t = 0;
	uint8_t counter = 0;
	uint8_t temp = 0;

	while(counter < 100)
	{
		t += digitalRead(joystick_trigger) == 0;
		_delay_us(1);
		++counter;
	}
	temp = t == counter; // Was the trigger held firm during the duration?
	send_packet[0] = x;
	send_packet[1] = y;
	send_packet[2] = z;
	send_packet[3] = 0;

	if(temp)
	{
		++trigger_hold_count;

		// Send Turbo
		if(!trigger_still_held && trigger_hold_count >= TURBO_COUNT)
		{
			send_packet[3] = 2;
			trigger_still_held = 1;
		}
	}
	else
	{
		// Send Vacuum
		if(trigger_hold_count >= VACUUM_COUNT && trigger_hold_count < TURBO_COUNT)
		{
			send_packet[3] = 1;
		}
		trigger_still_held = 0;
		trigger_hold_count = 0;
	}
	sprintf(buf, "Dir: X:%d Y:%d Z:%d T:%d\r\n", send_packet[0], send_packet[1], send_packet[2], send_packet[3]);
	writeUART(buf, BUFLEN);

	Radio.write(send_packet);

	delay(200);

#endif
}

#ifdef ECLIPSE_BUILD
int main(void)
{
	init();

	setup();

	for (;;)
		loop();

	return 0;
}
#endif
