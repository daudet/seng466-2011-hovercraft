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
    for (;;)
    {
    }
}
