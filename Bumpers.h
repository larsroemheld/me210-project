/**************************************************************
  File:      Bumpers.h
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our bumpers

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#ifndef H_BUMPERS
#define H_BUMPERS

#include "Arduino.h"


#define PIN_LEFT_BACKBUMPER   4
#define PIN_RIGHT_BACKBUMPER  11
#define PIN_LEFT_FRONTBUMPER  12
#define PIN_RIGHT_FRONTBUMPER 13


void initBumpers();

unsigned char isLeftBackBumperPressed();
unsigned char isRightBackBumperPressed();
unsigned char isAnyBackBumperPressed();

unsigned char isLeftFrontBumperPressed();
unsigned char isRightFrontBumperPressed();
unsigned char isAnyFrontBumperPressed();

#endif
