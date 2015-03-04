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

#define PIN_FRONT_SONAR_TRIGGER   9
#define PIN_FRONT_SONAR_ECHO      10

#define PIN_LEFT_SONAR_TRIGGER    2
#define PIN_LEFT_SONAR_ECHO       2


// Maximum range to be sensed by sonar in cm
#define MAX_SONAR_RANGE_CM     200

void initSonar();

int debugSonar(void);

unsigned char getSonarFrontDistanceInInches(unsigned char numPings);
unsigned char getSonarLeftDistanceInInches(unsigned char numPings);

#endif
