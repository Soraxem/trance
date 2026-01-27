/*
  Device:         Trance PIXEL-LED Prototype
  Board:          ESP32C3 Dev Module
  Board Library:  esp32 v3.0.4
  Dependencies:   trance v0.0.3, ArtNet v0.9.2, FastLED v3.10.3, IRremote v4.5.0
  Author:         Samuel Hafen
*/

#include <Arduino.h>
#include <Trance.h>
#include <FastLED.h>
#include <IRremote.hpp>



// Prototype because compiler sad
void callback(uint8_t test[]);

#define NUM_LEDS 50

#define DATA_PIN 4
CRGB leds[NUM_LEDS];

// Include Remote Control handling
#include "remote.h"

void setup() {
  // setup Trance
  delay(1000);

  Trance.begin(150, callback, 9);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  IrReceiver.begin(3, false);

  Serial.begin(115200);
}

void loop() {
  Trance.handle();
  FastLED.show();
  handle_infrared();
  handle_modes();
  delay(5);  //allow the cpu to switch to other tasks
}

// recieves data from Trance
void callback(uint8_t channels[]) {
  if (mode == Mode::TRANCE_MODE) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(channels[i*3], channels[i*3+1], channels[i*3+2]);
    }
  }
}

