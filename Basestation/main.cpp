/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "Core/WProgram.h"
#include "avr/interrupt.h"

extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}

int main()
{
    init();
    pinMode(13, OUTPUT);
    for(;;){
    	digitalWrite(13,HIGH);
    	delay(1000);
    	digitalWrite(13,LOW);
    	delay(1000);
    }
    for (;;) {}
    return 0;
}
