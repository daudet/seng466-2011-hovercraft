/*
 * sonar.cpp
 *
 *  Created on: Feb 08, 2011
 *      Author: degnanh
 */

#include "../Core/WProgram.h"
#include "sonar.h"

extern char sonarState;
extern unsigned int sonar[];
extern char currentSonar;

unsigned long sonarLastPing;

//set input capture direction, set sonar RX pins high, start timer4
void sonarInit()
{
	pinMode(48, INPUT);
	for (int i=0; i<NUM_SONARS; i++) {
		pinMode(SONAR_PIN+i, OUTPUT);
		digitalWrite(SONAR_PIN+i, 0);
	}
	//enable noise filter (uses 4 cpu clock cycles - negligible)
	//uses default 'falling edge' trigger
	//prescaler of 64 so we don't overflow our counter after 50ms
	TCCR4A = 0x00;
	TCCR4B = _BV(ICNC4) | _BV(CS41) | _BV(CS40);
	TCCR4C = 0x00;
}


//update what sonar we're currently ranging
void sonarUpdate()
{
	switch (sonarState) {
		case SONAR_POWERUP:
			digitalWrite(13, 0);
			if (millis() > 250) {
				sonarState = SONAR_CALIBRATE;
				//start the calibration
				sonarPing();
			}
		break;

	  case SONAR_CALIBRATE:
		  //power them all in series for 100ms
		  if (millis() - sonarLastPing > 100) {
			  currentSonar++;
			  if (currentSonar > NUM_SONARS) {
				  //start ranging, enable interrupts
				  currentSonar = 0;
				  sonarState = SONAR_RANGING;
				  TIMSK4 |= _BV(ICIE4);
				  TIFR4 |= _BV(ICF4);
				  break;
			  }
			  //otherwise start the next sonar calibrating
			  sonarPing();
		  }
		  break;

	  case SONAR_RANGING:
		digitalWrite(13, 1);
		//ping them in series every 50ms
		if (millis() - sonarLastPing > 50) {
		  currentSonar++;
		  if (currentSonar > NUM_SONARS)
			currentSonar = 0;
		  sonarPing();
		}
		break;
	}
}

//send a 30us low pulse to the current sonar
void sonarPing()
{
	digitalWrite(SONAR_PIN+currentSonar, 1);
	delayMicroseconds(25);
	TCNT4 = 0;
	TIFR4 |= _BV(ICF4);
	digitalWrite(SONAR_PIN+currentSonar, 0);
	sonarLastPing = millis();
}

