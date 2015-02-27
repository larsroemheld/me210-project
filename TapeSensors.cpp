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
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_LEFT_TAPESENSOR);
        //Serial.println("LeftSensor Reading:");
        //Serial.println(sensorVal);

        
	result = (sensorVal > TAPE_SENSOR_THRESHOLD_LEFT) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char isRightSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_RIGHT_TAPESENSOR);
        //Serial.println("RightSensor Reading:");
        //Serial.println(sensorVal);

	result = (sensorVal > TAPE_SENSOR_THRESHOLD_RIGHT) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char areBothSensorsOnTape() {
	return isLeftSensorOnTape() && isRightSensorOnTape();
}

