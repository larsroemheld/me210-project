/**************************************************************
  File:      Bumpers.h
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our bumpers

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#ifndef H_TAPESENSORS
#define H_TAPSENSORS

#include "Arduino.h"


#define PIN_LEFT_TAPESENSOR   5
#define PIN_RIGHT_TAPESENSOR  6

void initTapeSensors();

unsigned char isLeftSensorOnTape();
unsigned char isRightSensorOnTape();
unsigned char areBothSensorsOnTape();


#endif