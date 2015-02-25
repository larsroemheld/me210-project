/**************************************************************
  File:      Bumpers.c
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Read our bumpers

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

#include "Bumpers.h"

void initBumpers() {
	pinMode(PIN_LEFT_BACKBUMPER, INPUT);
	pinMode(PIN_RIGHT_BACKBUMPER, INPUT);
	pinMode(PIN_LEFT_FRONTBUMPER, INPUT);
	pinMode(PIN_RIGHT_FRONTBUMPER, INPUT);

}

unsigned char isLeftBackBumperPressed() {
	return (digitalRead(PIN_LEFT_BACKBUMPER) == HIGH) ? true : false;
}
unsigned char isRightBackBumperPressed() {
	return (digitalRead(PIN_RIGHT_BACKBUMPER) == HIGH) ? true : false;
}
unsigned char isAnyBackBumperPressed() {
	return (isLeftBackBumperPressed() || isRightBackBumperPressed());
}

unsigned char isLeftFrontBumperPressed() {
	return (digitalRead(PIN_LEFT_FRONTBUMPER) == HIGH) ? true : false;
}
unsigned char isRightFrontBumperPressed() {
	return (digitalRead(PIN_RIGHT_FRONTBUMPER) == HIGH) ? true : false;
}
unsigned char isAnyFrontBumperPressed() {
	return (isLeftFrontBumperPressed() || isRightFrontBumperPressed());
}

