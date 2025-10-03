#include <Trance.h>

// Prototype because compiler sad
void callback(uint8_t test[]);

Trance trance(3, callback);

void setup() {
  // setup Trance
  trance.begin();

  // setup PWM LED Pins
  pinMode(1, OUTPUT); // red
  pinMode(2, OUTPUT); // green
  pinMode(4, OUTPUT); // blue
}

void loop() {
  trance.handle();
}

// recieves data from Trance
void callback(uint8_t channels[]) {
  analogWrite(1, channels[0]);
  analogWrite(2, channels[1]);
  analogWrite(4, channels[2]);
}
