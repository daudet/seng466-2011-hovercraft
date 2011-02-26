/*
WiiClassic Test Code

This code prints the current controller status to the serial port.
Button pressed calls poll whether the button was pressed since the last update call.
as a result, it will just briefly print the last button pressed once.

Tim Hirzel May 2008

*/

#include <Wire.h>

#include "WiiClassic.h"

WiiClassic wiiClassy = WiiClassic();

void setup() {
  Wire.begin();
  Serial.begin(9600);
  wiiClassy.begin();
  wiiClassy.update();
}

void loop() {
  delay(1000); 
  wiiClassy.update();

  Serial.print("Buttons:");

  if (wiiClassy.leftShoulderPressed()) {
    Serial.print("LS.");
  }
  if (wiiClassy.rightShoulderPressed()) {
    Serial.print("RS.");
  }
  if (wiiClassy.lzPressed()) {
    Serial.print("lz.");
  }
  if (wiiClassy.rzPressed()) {
    Serial.print("rz.");
  }
  if (wiiClassy.leftDPressed()) {
    Serial.print("LD.");
  }

  if (wiiClassy.rightDPressed()) {
    Serial.print("RD.");
  }

  if (wiiClassy.upDPressed()) {
    Serial.print("UD.");
  }
  if (wiiClassy.downDPressed()) {
    Serial.print("DD.");
  }

  if (wiiClassy.selectPressed()) {
    Serial.print("select.");
  }

  if (wiiClassy.homePressed()) {
    Serial.print("home.");
  }
  if (wiiClassy.startPressed()) {
    Serial.print("start.");
  }

  if (wiiClassy.xPressed()) {
    Serial.print("x.");
  }

  if (wiiClassy.yPressed()) {
    Serial.print("y.");
  }

  if (wiiClassy.aPressed()) {
    Serial.print("a.");
  }

  if (wiiClassy.bPressed()) {
    Serial.print("b.");
  }

  Serial.println();
  Serial.print("right shoulder: ");
  Serial.println(wiiClassy.rightShouldPressure());
  Serial.print(" left shoulder: ");

  Serial.println(wiiClassy.leftShouldPressure());
  Serial.print("  left stick x: ");

  Serial.println(wiiClassy.leftStickX());
  Serial.print("  left stick y: ");

  Serial.println(wiiClassy.leftStickY());
  Serial.print(" right stick x: ");

  Serial.println(wiiClassy.rightStickX());
  Serial.print(" right stick y: ");

  Serial.println(wiiClassy.rightStickY());
  Serial.println("---");

}
