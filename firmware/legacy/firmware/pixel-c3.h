/*

Hardware implementation for Trance firmware

*/

#define NETWORK WIFI


#define NUM_LEDS 50
#define INTERFACE_CHANNELS NUM_LEDS*3

CRGB leds[NUM_LEDS]; // All Leds in an array

void interface_setup() {
  DEBUG_PRINTLN("INTERFACE: setting up");
  FastLED.addLeds<WS2812, 4, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void interface_handle() {
  // there is nothing to handle
}

void interface_call(int channels[INTERFACE_CHANNELS]) {
  DEBUG_PRINTLN("INTERFACE: got called");
  for (int i = 0; i < INTERFACE_CHANNELS; i = i + 3) {
    leds[i/3] = CRGB(channels[i], channels[i+1], channels[i+2]);
    DEBUG_PRINTLN(channels[i]);
  }
  FastLED.show();
}
