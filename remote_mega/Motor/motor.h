/*
 * motor.h
 *
 *  Created on: Jan 28, 2011
 *      Author: cei
 */

#include "../Core/WProgram.h"

//movement motors are on timer4
#define leftE	6  	//left
#define left1   37 	//left
#define left2   36 	//left
#define rightE  7 	//right
#define right1  35 	//right
#define right2  34 	//right
#define strafeE 8	//strafe
#define strafe1 23  //strafe
#define strafe2 22  //strafe
#define DEADZONE 160

//lift motors are on timer3
#define backE 2 //back lift
#define frontE 3 //front lift
#define frontMax 255 //(frontMax/liftStep) must be integer
#define backMax 255 //(backMax/liftStep) must be integer
#define liftStep 15

void motorinit();
void updateRight(int8_t);
void updateLeft(int8_t);
void updateStrafe(int8_t);


