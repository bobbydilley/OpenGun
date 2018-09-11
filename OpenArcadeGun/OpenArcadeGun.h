/*
  OpenArcadegun.h - Library for getting screen position values from an MPU-2650
  Created by Robert A. Dilley, September 9th, 2018.
  Released into the public domain.
*/

#ifndef OpenArcadeGun_h
#define OpenArcadeGun_h

#include "Arduino.h"

class OpenArcadeGun {
  public:
    OpenArcadeGun(int pin);
    int[] getPosition()
  private:
    int _pin;
};

#endif
