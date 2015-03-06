/**************************************************************
  File:      TapeSensors.h
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our tape sensors (IR Sender + Emitter pairs)

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#ifndef H_TAPESENSORS
#define H_TAPESENSORS

#include "Arduino.h"


#define PIN_LEFT_TAPESENSOR   A1
#define PIN_RIGHT_TAPESENSOR  A0
#define PIN_FRONT_TAPESENSOR  A2

#define MAX_ANALOG_READ 1023

#define TAPE_SENSOR_THRESHOLD_LEFT   100
#define TAPE_SENSOR_THRESHOLD_RIGHT  200
#define TAPE_SENSOR_THRESHOLD_FRONT  100
#define TAPE_SENSOR_HYSTERESIS_LEFT   0
#define TAPE_SENSOR_HYSTERESIS_RIGHT  0
#define TAPE_SENSOR_HYSTERESIS_FRONT  0



void initTapeSensors();

unsigned char isLeftSensorOnTape();
unsigned char isRightSensorOnTape();
unsigned char isFrontSensorOnTape();
unsigned char areAllSensorsOnTape();


#endif