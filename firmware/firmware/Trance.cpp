/*
  Trance.cpp Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal
*/

#include "Arduino.h"
#include "Trance.h"
#include "Network.h"

Trance::Trance(int pin) { 
	_test = pin;
}

void Trance::begin() {
	pinMode(_test, OUTPUT);
}

void Trance::handle() {
	digitalWrite(_test, HIGH);
	delay(250);
	digitalWrite(_test, LOW);
	delay(250);
}