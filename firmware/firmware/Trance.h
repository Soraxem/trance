/*
  Trance.h Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal
*/

#ifndef Trance_h
#define Trance_h

#ifndef TRANCE_ETHERNET  
#define TRANCE_WIFI true
#endif

#include "Arduino.h"

class Trance
{
  public:
    Trance(int pin);
    void begin();
    void handle();
  private:
    int _test;
};

#endif