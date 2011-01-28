/*
 * sonar.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: cei
 */

#include "sonar.h"

void sonarinit()
{
	pinMode(PWL, INPUT); //PWM input left
	pinMode(PWR, INPUT); //PWM input right
	delay(100); //100 ms to wait for first reading
}
