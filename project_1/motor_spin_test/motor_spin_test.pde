/*
  Test the motor driver
  Using Seeduino Mega
*/

void setup() {                
  //PWM outputs
  pinMode(2, OUTPUT);     
  pinMode(3, OUTPUT);
  //Motor directions
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);

  //low speed
  analogWrite(2, 100);
  analogWrite(3, 100);
  delay(2000);
  
  //high speed
  analogWrite(2, 255);
  analogWrite(3, 255);
  delay(2000);

  //coast  
  analogWrite(2, 100);
  analogWrite(3, 100);
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  delay(500);
  
  //low speed other way
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);
  delay(2000);
  
  //high speed
  analogWrite(2, 255);
  analogWrite(3, 255);
  delay(3000);
}
