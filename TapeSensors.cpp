/**************************************************************
  File:      TapeSensors.c
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our tape sensors (IR Sender + Emitter pairs)

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#include "TapeSensors.h"


static unsigned int threshold_left_low = TAPE_SENSOR_THRESHOLD_LEFT - TAPE_SENSOR_HYSTERESIS_LEFT;
static unsigned int threshold_left_high = TAPE_SENSOR_THRESHOLD_LEFT + TAPE_SENSOR_HYSTERESIS_LEFT;
static unsigned int threshold_right_low = TAPE_SENSOR_THRESHOLD_RIGHT - TAPE_SENSOR_HYSTERESIS_RIGHT;
static unsigned int threshold_right_high = TAPE_SENSOR_THRESHOLD_RIGHT + TAPE_SENSOR_HYSTERESIS_RIGHT;
static unsigned int threshold_front_low = TAPE_SENSOR_THRESHOLD_FRONT - TAPE_SENSOR_HYSTERESIS_FRONT;
static unsigned int threshold_front_high = TAPE_SENSOR_THRESHOLD_FRONT + TAPE_SENSOR_HYSTERESIS_FRONT;

void initTapeSensors() {
	pinMode(PIN_RIGHT_TAPESENSOR, INPUT);
	pinMode(PIN_LEFT_TAPESENSOR, INPUT);
	pinMode(PIN_FRONT_TAPESENSOR, INPUT);
}

unsigned char isLeftSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;
	unsigned int threshold;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_LEFT_TAPESENSOR);

	if (lastReadingOnTape) {
		threshold = threshold_left_low;
	} else {
		threshold = threshold_left_high;
	}
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

	if (lastReadingOnTape) {
		threshold = threshold_right_low;
	} else {
		threshold = threshold_right_high;
	}
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

	if (lastReadingOnTape) {
		threshold = threshold_front_low;
	} else {
		threshold = threshold_front_high;
	}
	result = (sensorVal > threshold) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char areAllSensorsOnTape() {
	return isLeftSensorOnTape() && isRightSensorOnTape() && isFrontSensorOnTape();
}

