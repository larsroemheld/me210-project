/**************************************************************
  File:      TapeSensors.c
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our tape sensors (IR Sender + Emitter pairs)

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#include "TapeSensors.h"


void initTapeSensors() {
	pinMode(PIN_RIGHT_TAPESENSOR, INPUT);
	pinMode(PIN_LEFT_TAPESENSOR, INPUT);
}

// TODO: Threshold values, hysteresis (testing)
unsigned char isLeftSensorOnTape() {

}
unsigned char isRightSensorOnTape() {

}
unsigned char areBothSensorsOnTape() {
	return isLeftSensorOnTape() && isRightSensorOnTape();
}

