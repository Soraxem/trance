/*

  Wireles control of a Neopixel strip. You have to get the Library Versions right.

  Board: esp32-c3 Supermini
  Board Library: esp32 v3.2.1
  Dependencies: FastLED v3.10.3

*/

#include <Arduino.h>

#include <trance.h>

#include <FastLED.h>

// Prototype because compiler sad
void callback(uint8_t test[]);

#define NUM_LEDS 50

#define DATA_PIN 4
CRGB leds[NUM_LEDS];

void setup() {
  // setup Trance
  delay(1000);

  Trance.begin(150, callback, 9);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void loop() {
  Trance.handle();
  FastLED.show();
  delay(5);  //allow the cpu to switch to other tasks
}

// recieves data from Trance
void callback(uint8_t channels[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(channels[i*3], channels[i*3+1], channels[i*3+2]);
  }
}
