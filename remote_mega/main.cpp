/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "Core/WProgram.h"
#include "avr/interrupt.h"
#include "Motor/motor.h"
#include "UART/UART.h"
#include "Sonar/sonar.h"

extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}

unsigned int sonar[NUM_SONARS];
char sonarState = SONAR_POWERUP;
char currentSonar;
//int t;

void liftRampUp(int, int);

void blink()
{
	pinMode(13, OUTPUT);

	for(;;){
	    digitalWrite(13,HIGH);
	    delay(500);
	    digitalWrite(13,LOW);
	    delay(500);
	}
}

void liftRampUp(int* frontVal, int* backVal){
	unsigned int t2 = 0;
	while((*frontVal < frontMax) && (*backVal < backMax)){
		if (millis() - t2 > 500){
			t2 = millis();
			*frontVal += liftStep;
			*backVal += liftStep;
		}
	}
}

int main(){

    init();

	motorinit();
	UARTinit();
	//blink();
	//sonarInit();

	motorinit();

    byte input[8];
    byte output[4];
	output[0] = 65;
	output[1] = 66;
	output[2] = 67;
	output[3] = 68;

	unsigned int t1 = 0;
	//set default values for lift motors
	byte frontVal = 0;
	byte backVal = 0;
	byte Lflag = 0; //1 if lift 0 if kill
	byte Upflag=0;
	byte Downflag=0;

	for(;;){


	for(;;)
	{
		if(UARTreceive(input,8))
		{
			updateRight((int8_t) input[0]);
			updateLeft((int8_t) input[2]);
			updateStrafe((int8_t) input[1]);

			if (input[3]==1)
				Lflag=(Lflag+1)%2;
			if (input[6]==1)
				Upflag=1;
			if (input[7]==1)
				Downflag=1;

			if (Upflag==1) {
				Upflag=0;
				//if the motors are stopped, ramp up to the max PWM values
				if((frontVal == 0) && (backVal == 0)){
					liftRampUp(&frontVal, &backVal);
				}

				//increase lift power if '+' button pressed
				if (input[6] == 1){
					if((frontVal < frontMax) && (backVal < backMax))
						frontVal += liftStep;
						backVal += liftStep;
				}

				//decrement lift power if '-' button pressed
				if(input[7] == 1){
					if((frontVal > frontMin) && (backVal > backMin))
					frontVal -= liftStep;
					backVal -= liftStep;
				}
			}
			if (Downflag==1) {
				Downflag=0;
				if (frontVal>0)
					frontVal-=liftStep;
				if (backVal>0)
					backVal-=liftStep;
			}

			analogWrite(backE,backVal); //lift back motor
			analogWrite(frontE,frontVal); //lift front motor

		}

		if (millis() - t > 100){
			t = millis();
			UARTsend(output,4);

//			if (Lflag) { //lift
//				if (frontVal<frontMax)
//					frontVal+=liftStep;
//				if (backVal<backMax)
//					backVal+=liftStep;
//			}
//			else { //lower
//				frontVal=0;
//				backVal=0;
//			}



		}
		//sonarUpdate();
		//output[0] = (byte) (sonar[0]/16);
		//output[1] = (byte) (sonar[1]/16);
	}
	return 0;
}

//our sonar ping has been received - save the TCNT

//ISR(TIMER4_CAPT_vect)
//{
//  char sreg = SREG;
//  sonar[currentSonar] = ICR4/64;
//  SREG = sreg;
//}
