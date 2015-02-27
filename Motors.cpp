/**************************************************************
  File:      Motors.c
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Power our motors

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#include "Motors.h"


void initMotors() {
	// "You do not need to call "pinMode(PIN_LEFT_MOTOR, OUTPUT);" before using analogWrite()

}

// Set speed of left motor, if newSpeed > (NULL_VALUE). Return old speed.
// TODO: how do we handle 2 directions? DIR pin or DC polarity?
signed int setLeftMotorSpeed(signed int newSpeed) {
	static signed int leftSpeed = 0;

	// NOTE: since motors are facing opposite directions, they need to be running in opposite directions to go forward. Hence this line.
    newSpeed = -newSpeed;

	int oldSpeed;
	oldSpeed = leftSpeed;
	if (newSpeed > NULL_VALUE) {
		leftSpeed = newSpeed;
 Serial.print(" LEFT SPEED NEW:");
 Serial.println(newSpeed);

		// Set arduino motor outputs to value
		analogWrite(PIN_LEFT_MOTOR_SPEED, (byte) abs(newSpeed));
		digitalWrite(PIN_LEFT_MOTOR_DIR, (newSpeed >= 0) ? HIGH : LOW);
	}
	return oldSpeed;
}

// Set speed of right motor, if newSpeed > (NULL_VALUE). Return old speed.
// TODO: how do we handle 2 directions? DIR pin or DC polarity?
signed int setRightMotorSpeed(signed int newSpeed) {
	static signed int rightSpeed = 0;

	int oldSpeed;
	oldSpeed = rightSpeed;
	if (newSpeed > NULL_VALUE) {
		rightSpeed = newSpeed;

 Serial.print(" RIGHT SPEED NEW:");
 Serial.println(newSpeed);

		// Set arduino motor outputs to value
		analogWrite(PIN_RIGHT_MOTOR_SPEED, (byte) abs(newSpeed));
		digitalWrite(PIN_RIGHT_MOTOR_DIR, (newSpeed >= 0) ? HIGH : LOW);
	}
	return oldSpeed;
}

void setMotorSpeed(signed int newSpeed) {
  setLeftMotorSpeed(newSpeed);
  setRightMotorSpeed(newSpeed);
}


