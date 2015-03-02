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

unsigned char isLeftSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;
	unsigned int threshold;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_LEFT_TAPESENSOR);

    threshold = TAPE_SENSOR_THRESHOLD_LEFT + (lastReadingOnTape == false) ? TAPE_SENSOR_HYSTERESIS_LEFT : -TAPE_SENSOR_HYSTERESIS_LEFT;
	result = (sensorVal > threshold) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char isRightSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;
	unsigned int threshold;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_RIGHT_TAPESENSOR);

    threshold = TAPE_SENSOR_THRESHOLD_RIGHT + (lastReadingOnTape == false) ? TAPE_SENSOR_HYSTERESIS_RIGHT : -TAPE_SENSOR_HYSTERESIS_RIGHT;
	result = (sensorVal > threshold) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char isFrontSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;
	unsigned int threshold;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_FRONT_TAPESENSOR);

    threshold = TAPE_SENSOR_THRESHOLD_FRONT + (lastReadingOnTape == false) ? TAPE_SENSOR_HYSTERESIS_FRONT : -TAPE_SENSOR_HYSTERESIS_FRONT;
	result = (sensorVal > threshold) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char areAllSensorsOnTape() {
	return isLeftSensorOnTape() && isRightSensorOnTape() && isFrontSensorOnTape();
}

