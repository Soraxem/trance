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
void handle_infrared();
void handle_modes();

#define NUM_LEDS 50

#define DATA_PIN 4
CRGB leds[NUM_LEDS];


namespace Mode {
  enum Mode {
    TRANCE_MODE,
    BLACK_OUT,
    PROGRAM,
    MANUAL,
  };
}

namespace Program {
  enum program {
    RAINBOW,
    ABSTRACT,
    FIRE,
    OCEAN,
    LINE,
    COUNT
  };

  program current = RAINBOW;

  uint8_t animation_state = 0;
  uint8_t animation_speed = 15;

  int last_millis = 0;

  void next() {
    // 1. Convert to int and increment
    // 2. Use modulo to wrap around (3 % 3 = 0)
    // 3. Cast back to the 'program' type
    current = static_cast<program>((current + 1) % COUNT);
  }
}

namespace Manual {
  enum color {
    RED,
    GREEN,
    BLUE,
  };

  uint8_t red_level = 0;
  uint8_t green_level = 0;
  uint8_t blue_level = 0;

  color selected;
}

// create mode variable with standart value TRANCE_MODE
Mode::Mode mode = Mode::TRANCE_MODE;


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

// Eurolie IR-7 Remote Control Commands
// Values are arranged in a Table as the buttons are arranged
namespace IR_7 {
enum Key {
  BLACK_OUT = 0,  AUTO = 1,     SOUND = 2,
  STROBE = 3,     SPEED = 4,    SENSITIVITY = 5,
  DMX = 6,        MANUAL = 7,   FADE = 8,

  RED = 9,        GREEN = 10,   BLUE = 11,

  PLUS = 12,      ZERO = 13,    MIUNS = 14,
  ONE = 15,       TWO = 16,     THREE = 17,
  FOUR = 18,      FIVE = 19,    SIX = 20,
  SEVEN = 21,     EIGHT = 22,   NINE = 23,
};
}

namespace Remote {
  enum Key {
    // Row 1 (Top Controls)
    BRIGHT = 0, 
    DIM = 1, 
    OFF = 2, 
    ON = 3,

    // Row 2 (Primary Colors)
    RED = 4, 
    GREEN = 5, 
    BLUE = 6, 
    WHITE = 7,

    // Row 3 (Secondary Colors)
    PURPLE = 8, 
    ORANGE = 9, 
    YELLOW = 10, 
    CYAN = 11,

    // Row 4 (Effects & Music)
    SPEED_UP = 12,    // Lightning icon with +
    SETTINGS = 13,    // Sliders/EQ icon
    SWITCH_DIR = 14,  // Left/Right arrows
    MUSIC_UP = 15,    // Music note with +

    // Row 5 (Adjustments)
    SPEED_DOWN = 16,  // Lightning icon with -
    PALETTE = 17,     // Paint palette icon
    CYCLE = 18,       // Circular recycle arrows
    MUSIC_DOWN = 19,  // Music note with -

    // Row 6 (Timers & Modes)
    TIMER_4H = 20,    // Clock with 4
    TIMER_8H = 21,    // Clock with 8
    MODE_1 = 22,
    MODE_2 = 23
  };
}

void handle_infrared() {
  if (IrReceiver.decode()) {
    Serial.print("Got Infrared! command: ");
    Serial.println(IrReceiver.decodedIRData.command);

    bool repeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;

    // Check for Eurolie IR-7 Remote Control
    if (IrReceiver.decodedIRData.address == 61706) {
      switch (IrReceiver.decodedIRData.command) {
        case IR_7::BLACK_OUT:
          mode = Mode::BLACK_OUT;
          break;
        case IR_7::AUTO:
          mode = Mode::PROGRAM;
          if (!repeat) {
            Program::next();
          }
          break;
        case IR_7::DMX:
          mode = Mode::TRANCE_MODE;
          break;
        case IR_7::MANUAL:
          mode = Mode::MANUAL;
          break;

        case IR_7::RED:
          Manual::selected = Manual::RED;
          break;
        
        case IR_7::GREEN:
          Manual::selected = Manual::GREEN;
          break;
        
        case IR_7::BLUE:
          Manual::selected = Manual::BLUE;
          break;

        case IR_7::MIUNS:
          if (mode == Mode::MANUAL) {
            switch (Manual::selected) {
              case Manual::RED:
                if (Manual::red_level > 0) Manual::red_level--;
                break;
              case Manual::GREEN:
                if (Manual::green_level > 0) Manual::green_level--;
                break;
              case Manual::BLUE:
                if (Manual::blue_level > 0) Manual::blue_level--;
                break;
            }
          }
          break;

        case IR_7::PLUS:
          if (mode == Mode::MANUAL) {
            switch (Manual::selected) {
              case Manual::RED:
                if (Manual::red_level < 9) Manual::red_level++;
                break;
              case Manual::GREEN:
                if (Manual::green_level < 9) Manual::green_level++;
                break;
              case Manual::BLUE:
                if (Manual::blue_level < 9) Manual::blue_level++;
                break;
            }
          }
          break;
        
        default:
          //Serial.println("Unknown Key pressed.");
          break;
      }
    }

    IrReceiver.resume();
  }
}

void handle_modes() {

  if (mode == Mode::MANUAL) {
    CRGB color = CRGB(Manual::red_level * 28, Manual::green_level * 28, Manual::blue_level * 28);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
  }

  if (mode == Mode::BLACK_OUT) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  }

  if (mode == Mode::PROGRAM) {

      switch (Program::current) {
        case (Program::RAINBOW):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(i*4 + beat8(Program::animation_speed), 255, 255);
          }
          break;

        case (Program::ABSTRACT):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(Program::animation_speed, 140, 255, 0, i*4), 180, 255);
          }
          break;

        case (Program::FIRE):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(Program::animation_speed, 0, 60, 0, i*5), 250, beatsin8(Program::animation_speed, 150, 255, 0, i*7));
          }
          break;
        
        case (Program::OCEAN):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(Program::animation_speed, 120, 170, 0, i*2), beatsin8(Program::animation_speed, 180, 255, 0, i*2), 255);
          }
          break;
        
        case (Program::LINE):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beat8(Program::animation_speed), 255, beatsin8(Program::animation_speed, 0, 255, 0, i*51 + beat8(Program::animation_speed)) );
          }
          break;
      }
  }

}

