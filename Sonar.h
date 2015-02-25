/**************************************************************
  File:      Sonar.h
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read sonar sensors
// https://code.google.com/p/arduino-new-ping/
// Install library in arduino folder!

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/


#ifndef H_BUMPERS
#define H_BUMPERS

#include "NewPing.h"

#define PIN_LEFT_SONAR_TRIGGER   0
#define PIN_LEFT_SONAR_ECHO      1

// Maximum range to be sensed by sonar in cm
#define MAX_SONAR_RANGE_CM     200

// Global object; needed to be global so all internal functions work with this
// object without coupling the sensor to other parts of code
NewPing Sonar_LeftSonar(PIN_LEFT_SONAR_TRIGGER, PIN_LEFT_SONAR_ECHO, MAX_SONAR_RANGE_CM);

void initSonar();

unsigned char getSonarLeftDistanceInCM(unsigned char numPings);

#endif
