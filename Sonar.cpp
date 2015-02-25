/**************************************************************
  File:      Sonar.cpp
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read sonar sensors
// https://code.google.com/p/arduino-new-ping/
// Install library in arduino folder!

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#include "Sonar.h"


void initSonar() {
//	NewPing Sonar_LeftSonar(PIN_LEFT_SONAR_TRIGGER, PIN_LEFT_SONAR_ECHO, MAX_SONAR_RANGE_CM);
}

unsigned char getSonarLeftDistanceInCM(unsigned char numPings) {
	unsigned int medianSonarMicroSeconds, medianSonarCM;

//	medianSonarMicroSeconds = Sonar_LeftSonar.ping_median(numPings);
//	medianSonarCM = Sonar_LeftSonar.convert_cm(medianSonarMicroSeconds);

	return medianSonarCM;
}
