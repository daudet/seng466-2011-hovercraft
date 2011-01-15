/*
 * main.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: cei
 */

#include <stdlib.h>
#include "WProgram.h"
#include "avr/interrupt.h"
extern "C" void __cxa_pure_virtual()
{
    cli();    // disable interrupts
    for(;;);  // do nothing until hard reset
}


int main()
{
    init();
    pinMode(13,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(11,OUTPUT);
    analogWrite(13,10);
    digitalWrite(12,LOW);
    digitalWrite(11,HIGH);
    for (;;) {}
    return 0;
}
