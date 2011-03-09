/*
 * motor.h
 *
 *  Created on: Jan 28, 2011
 *      Author: cei
 */

#include "../Core/WProgram.h"

#define E1_2 3 //left
#define E3_4 2 //right
#define L1   10 //left
#define L2   11 //left
#define L3   8 //right
#define L4   9 //right
#define DEADZONE 160

#define LE1 4
#define LE2 5
#define LMAX1 15 //(255-LMAX)/LSTEP must be integer
#define LMAX2 15 //(255-LMAX)/LSTEP must be integer
#define LSTEP 10 //(255-LMAX)/LSTEP must be integer

void motorinit();
void updateRight(int8_t);
void updateLeft(int8_t);


