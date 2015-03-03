/**************************************************************
  File:      Motors.h
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Power our motors

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#ifndef H_MOTORS
#define H_MOTORS

#include "Arduino.h"

#define NULL_VALUE -10000 // used as threshold for non-setting function calls
#define MAX_MOTOR_SPEED 200 // 254

#define PIN_LEFT_MOTOR_SPEED 5
#define PIN_RIGHT_MOTOR_SPEED 6
#define PIN_LEFT_MOTOR_DIR 7
#define PIN_RIGHT_MOTOR_DIR 8`


void initMotors();

// Set speed of left motor, if newSpeed > (NULL_VALUE). Return old speed.
signed int setLeftMotorSpeed(signed int newSpeed);
// Set speed of right motor, if newSpeed > (NULL_VALUE). Return old speed.
signed int setRightMotorSpeed(signed int newSpeed);
// Set both motor speeds, return nothing.
void setMotorSpeed(signed int newSpeed);
// Whatever speed both motors have, reverse them at the same speed.
void reverseMotors();

#endif
