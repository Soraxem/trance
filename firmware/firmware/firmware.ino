

#include "Trance.h"

Trance trance(3);

void setup() {
  trance.begin();
}

void loop() {
  trance.handle();
}
