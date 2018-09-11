/*
  OpenArcadegun.cpp - Library for getting screen position values from an MPU-2650
  Created by Robert A. Dilley, September 9th, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "OpenArcadeGun.h"

OpenArcadeGun::OpenArcadeGun(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

int[] OpenArcadeGun::getPosition() {
  int bobby[] = {1, 2};
  return bobby;
}
