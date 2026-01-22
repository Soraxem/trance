namespace Mode {
  enum Mode {
    // Default trance behavior
    TRANCE_MODE,
    // Turn everything off
    BLACK_OUT,
    // Run a Internal Program
    PROGRAM,
    // Let the user pick a color
    PALETTE,
  };
}

// create mode variable with standart value TRANCE_MODE
Mode::Mode mode = Mode::TRANCE_MODE;

const uint8_t level_max = 17;

uint8_t brightness = level_max;



namespace Program {
  enum program {
    FIRE, // Red Key
    RAINBOW, // Green Key
    OCEAN, // Blue Key
    ABSTRACT, // White Key
    LINE, // Purple Key
    COUNT
  };

  program current = FIRE;

  uint8_t speed_level = 1;

  long int last_millis = 0;

  void next() {
    // Convert to int and increment
    // Cast back to the program type
    current = static_cast<program>((current + 1) % COUNT);
  }
}

CRGB palette_color = CRGB(255,0,0);

// Unnamed Remote Control of Globe Lights
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
    Serial.println(IrReceiver.decodedIRData.address);

    bool repeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;

    if (IrReceiver.decodedIRData.address == 61184) {
      switch (IrReceiver.decodedIRData.command) {
        // --- Mode Buttons ---
        case Remote::PALETTE:
          mode = Mode::PALETTE;
          break;

        case Remote::SETTINGS:
          mode = Mode::PROGRAM;
          break;

        case Remote::SWITCH_DIR:
          mode = Mode::TRANCE_MODE;
          // Turn All LEDs off
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
          }
          // Reset Brightness
          FastLED.setBrightness(255);
          break;

        // --- Modifiers ---
        case Remote::BRIGHT:
          if (!Mode::TRANCE_MODE) {
            brightness = constrain(brightness+1, 1, level_max);
            FastLED.setBrightness(map(brightness, 0, 17, 0, 255));
          }
          break;
        case Remote::DIM:
          if (!Mode::TRANCE_MODE) {
            brightness = constrain(brightness-1, 1, level_max);
            FastLED.setBrightness(map(brightness, 0, 17, 0, 255));
          }
          break;
        case Remote::SPEED_UP:
          if (Mode::PROGRAM) {
            Program::speed_level = constrain(Program::speed_level+1, 1, level_max);
          }
          break;
        case Remote::SPEED_DOWN:
          if (Mode::PROGRAM) {
            Program::speed_level = constrain(Program::speed_level-1, 1, level_max);
          }
          break;

        // --- Row 2: Primary Colors ---
        case Remote::RED:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Red;
              break;
            case Mode::PROGRAM:
              Program::current = Program::FIRE;
              break;
          }
          break;

        case Remote::GREEN:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Green;
              break;
            case Mode::PROGRAM:
              Program::current = Program::RAINBOW;
              break;
          }
          break;

        case Remote::BLUE:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Blue;
              break;
            case Mode::PROGRAM:
              Program::current = Program::OCEAN;
              break;
          }
          break;

        case Remote::WHITE:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::White;
              break;
            case Mode::PROGRAM:
              Program::current = Program::ABSTRACT;
              break;
          }
          break;

        // --- Row 3: Secondary Colors ---
        case Remote::PURPLE:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Purple;
              break;
            case Mode::PROGRAM:
              Program::current = Program::LINE;
              break;
          }
          break;

        case Remote::ORANGE:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Orange;
              break;
          }
          break;

        case Remote::YELLOW:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Yellow;
              break;
          }
          break;

        case Remote::CYAN:
          switch (mode) {
            case Mode::PALETTE:
              palette_color = CRGB::Cyan;
              break;
          }
          break;
      }
    }
    IrReceiver.resume();
  }
}

void handle_modes() {

  if (mode == Mode::PALETTE) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = palette_color;
    }
  }

  if (mode == Mode::BLACK_OUT) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  }

  if (mode == Mode::PROGRAM) {
    int animation_speed = map(Program::speed_level, 1, 17, 8, 140);
    

      switch (Program::current) {
        case (Program::RAINBOW):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(i*4 + beat8(animation_speed), 255, 255);
          }
          break;

        case (Program::ABSTRACT):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(animation_speed, 140, 255, 0, i*9), 180, 255);
          }
          break;

        case (Program::FIRE):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(animation_speed, 0, 60, 0, i*5), 250, beatsin8(animation_speed, 150, 255, 0, i*7));
          }
          break;
        
        case (Program::OCEAN):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beatsin8(animation_speed, 120, 170, 0, i*2), beatsin8(animation_speed, 180, 255, 0, i*2), 255);
          }
          break;
        
        case (Program::LINE):
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(beat8(animation_speed), 255, beatsin8(animation_speed, 0, 255, 0, i*51 + beat8(animation_speed)) );
          }
          break;
      }
  }

}
