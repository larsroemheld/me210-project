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
	pinMode(PIN_FRONT_TAPESENSOR, INPUT);
}

unsigned char isLeftSensorOnTape() {
	static unsigned char lastReadingOnTape = false;
	static unsigned int lastReadingVal = 0;
	unsigned int threshold;

	unsigned int sensorVal; unsigned char result;

	sensorVal = analogRead(PIN_LEFT_TAPESENSOR);

	// if (lastReadingOnTape) {
	// 	threshold = TAPE_SENSOR_THRESHOLD_LEFT - TAPE_SENSOR_HYSTERESIS_LEFT;
	// } else {
	// 	threshold = TAPE_SENSOR_THRESHOLD_LEFT + TAPE_SENSOR_HYSTERESIS_LEFT;
	// }
	threshold = 90;
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

	// if (lastReadingOnTape) {
	// 	threshold = TAPE_SENSOR_THRESHOLD_RIGHT - TAPE_SENSOR_HYSTERESIS_RIGHT;
	// } else {
	// 	threshold = TAPE_SENSOR_THRESHOLD_RIGHT + TAPE_SENSOR_HYSTERESIS_RIGHT;
	// }
	threshold = 90;
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

	// if (lastReadingOnTape) {
	// 	threshold = TAPE_SENSOR_THRESHOLD_FRONT - TAPE_SENSOR_HYSTERESIS_FRONT;
	// } else {
	// 	threshold = TAPE_SENSOR_THRESHOLD_FRONT + TAPE_SENSOR_HYSTERESIS_FRONT;
	// }
	threshold = 90;
	result = (sensorVal > threshold) ? true : false;

	lastReadingVal = sensorVal;
	lastReadingOnTape = result;
	return result;
}

unsigned char areAllSensorsOnTape() {
	return isLeftSensorOnTape() && isRightSensorOnTape() && isFrontSensorOnTape();
}

