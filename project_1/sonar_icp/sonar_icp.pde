//sonar PWM pins connect to PL1, Pin 48 on seeeduino mega
//sonar 1 RX connect to pin 22
//sonar 2 RX connect to pin 23
//...

#define NUM_SONARS       2
#define SONAR_PIN        22
#define SONAR_POWERUP    0
#define SONAR_CALIBRATE  1
#define SONAR_RANGING    2

void sonarSetup();
void processSonar();
void pingSonar();

char sonarState = SONAR_POWERUP;
unsigned int sonar[NUM_SONARS];
char currentSonar;
unsigned long sonarLastPing;

unsigned long t;

void setup() {
  sonarSetup();
  t = millis();
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  
  for (int i=SONAR_PIN; i<SONAR_PIN+NUM_SONARS; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
}


void loop() {
  if (millis() - t > 100) {
    //Serial.print("Right sonar: ");
    //Serial.println(sonar[0]*10/15);
    //Serial.print("Left sonar:  ");
    Serial.println(sonar[1]);
   // Serial.println(" ");

    t = millis();
    /*if (sonar[0] > sonar[1])
      digitalWrite(13, 1);
    else
      digitalWrite(13, 0);*/
  }
  processSonar();
}



//set input capture direction, set sonar RX pins high, start timer4
void sonarSetup()
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
void processSonar()
{
  switch (sonarState) {
    case SONAR_POWERUP:
      digitalWrite(13, 0);
      if (millis() > 250) {
        sonarState = SONAR_CALIBRATE;
        //start the calibration
        pingSonar();
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
        pingSonar();
      }
      break;
      
    case SONAR_RANGING:
      digitalWrite(13, 1);
      //ping them in series every 50ms
      if (millis() - sonarLastPing > 50) {
        currentSonar++;
        if (currentSonar > NUM_SONARS)
          currentSonar = 0;
        pingSonar();
      }
      break;
  }
}

//send a 30us low pulse to the current sonar
void pingSonar()
{
  digitalWrite(SONAR_PIN+currentSonar, 1);
  delayMicroseconds(25);
  TCNT4 = 0;
  TIFR4 |= _BV(ICF4);
  digitalWrite(SONAR_PIN+currentSonar, 0);
  sonarLastPing = millis();
}

//our sonar ping has been received - save the TCNT
ISR(TIMER4_CAPT_vect)
{
  char sreg = SREG;
  sonar[currentSonar] = ICR4/64;
  SREG = sreg;
}

