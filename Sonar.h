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


#ifndef H_SONAR
#define H_SONAR

#include "NewPing.h"

#define PIN_LEFT_SONAR_TRIGGER   8
#define PIN_LEFT_SONAR_ECHO      7

// Maximum range to be sensed by sonar in cm
#define MAX_SONAR_RANGE_CM     200

void initSonar();

int debugSonar(void);

unsigned char getSonarLeftDistanceInInches(unsigned char numPings);

#endif
