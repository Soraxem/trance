#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ESPAsyncE131.h>
#include <Arduino.h>
#include <esp_dmx.h>

#include "webinterface.h"

// Configure Wifi
const char *ssid = "Connect House";
const char *password = "JesusIsLord";

// Create Webserver
WebServer server(80);

// Configure E1.31
#define UNIVERSE 1
#define UNIVERSE_COUNT 2
ESPAsyncE131 e131(UNIVERSE_COUNT);

// Configure DMX
int transmitPin = 4;
int receivePin = 1;
int enablePin = 3;
dmx_port_t dmxPort = 1; //pin 18

// Setup Signal LED
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 14

CRGB leds[NUM_LEDS];

// Setup Button
#define BUTTON 15

void handleRoot() {
  server.send(200, "text/html", home);
}

void handleNotFound() {
  server.send(404, "text/html", "File not Found");
}


void handleE131() {
  if (!e131.isEmpty()) {
        e131_packet_t packet;
        e131.pull(&packet);     // Pull packet from ring buffer
        
        dmx_write(dmxPort, packet.property_values, DMX_PACKET_SIZE);

        Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
                htons(packet.universe),                 // The Universe for this packet
                htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                e131.stats.num_packets,                 // Packet counter
                e131.stats.packet_errors,               // Packet error counter
                packet.property_values[1]);             // Dimmer data for Channel 1
    }
}


void setup(void) {
  //Start Debugging
  Serial.begin(115200);
  Serial.println("");
  
  // Setup Wifi client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  Serial.println("Connecting to Wifi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Debug Wifi info
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure Webserver
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  // Start Listening to a.snc
  if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))
    Serial.println(F("Listening for data..."));
  else 
    Serial.println(F("*** e131.begin failed ***"));

  // Setup DMX Driver
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {};
  int personality_count = 0;
  dmx_driver_install(dmxPort, &config, personalities, personality_count);
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);

  // Setup Fastled
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  // Setup Button
  pinMode(BUTTON, INPUT);
}

void loop(void) {
  server.handleClient(); // Handle Webrequests
  handleE131(); // Handle incoming a.SNC packets
  dmx_send_num(dmxPort, DMX_PACKET_SIZE);
  dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);

  if (digitalRead(BUTTON))
    leds[0] = CRGB::Red;
  else
    leds[0] = CRGB::Black;

  FastLED.show();
  
  delay(20);  //allow the cpu to switch to other tasks
}
