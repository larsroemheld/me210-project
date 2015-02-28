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

#define MAX_ANALOG_READ 1023

#define TAPE_SENSOR_THRESHOLD_LEFT 100
#define TAPE_SENSOR_THRESHOLD_RIGHT 100
#define TAPE_SENSOR_HYSTERESIS_LEFT  30
#define TAPE_SENSOR_HYSTERESIS_RIGHT 30



void initTapeSensors();

unsigned char isLeftSensorOnTape();
unsigned char isRightSensorOnTape();
unsigned char areBothSensorsOnTape();


#endif