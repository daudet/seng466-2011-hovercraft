/*
 * sonar.h
 *
 *  Created on: Feb 08, 2011
 *      Author: degnanh
 */

#define NUM_SONARS       2
#define SONAR_PIN        22

#define SONAR_POWERUP    0
#define SONAR_CALIBRATE  1
#define SONAR_RANGING    2

void sonarInit();
void sonarUpdate();
void sonarPing();


