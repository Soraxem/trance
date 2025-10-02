/*

  Boards: esp32 v3.0.4
  Dependencies: esp_dmx v4.1.0
*/

#include <Arduino.h>
#include <esp_dmx.h>

#include <trance.h>

int transmitPin = 4;
int receivePin = 1;
int enablePin = 3;

dmx_port_t dmxPort = 1;

uint8_t data[513];

// Prototype because compiler sad
void callback(uint8_t test[]);

void setup() {
  // setup Trance
  delay(2000);

  Trance.begin(512, callback, 0);

  // Setup DMX Driver
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {};
  int personality_count = 0;
  dmx_driver_install(dmxPort, &config, personalities, personality_count);
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
}

void loop() {
  Trance.handle();

  dmx_write(dmxPort, data, DMX_PACKET_SIZE);
  dmx_send_num(dmxPort, DMX_PACKET_SIZE);
  dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);
  delay(5);  //allow the cpu to switch to other tasks
}

// recieves data from Trance
void callback(uint8_t channels[]) {
  memcpy(data, channels, sizeof(data));
}
