/*

Firmware for the Trance hardware.

Prequisites:
Boards: esp32 v3.2.1
Librarys: ESPAsyncWebServer v3.7.10, AsyncTCP v3.4.5

Upload Settings:
Board: ESP32-S3-USB-OTG
Upload Mode: UART0 / Hardware CDC
USB Mode: Hardware CDC and JTAG

Author: Samuel Hafen

*/



// Comment or uncomment this line to enable/disable debugging
#define DEBUG
// Create macros for serial debugging
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Allows for persisten Preferences storage
#include <Preferences.h>
Preferences preferences;

// Library for connecting to Wifi networks
#include <WiFi.h>

// Resources for Captive Portal and Configuration
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
static DNSServer dnsServer;
static AsyncWebServer server(80);

// Device initialisation
void setup() {

  // Activate serial communication when debugging is enabled
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  DEBUG_PRINTLN("-- Start Debugging --");

  load_network_preferences();
  start_wifi();

  server.on("/", HTTP_GET, on_config);
  server.onNotFound(onRequest);
  server.begin();

}

void onRequest(AsyncWebServerRequest *request){
  request->redirect("http://" + WiFi.softAPIP().toString() );
}

void on_config(AsyncWebServerRequest *request){
  request->send(200, "text/plain", "Hello World!");
}

// Load Network Preferences, start_wifi has to be run to apply these Preferences
void load_network_preferences() {
  // Loading Network Preferences
  preferences.begin("network", true);
  bool dhcp = preferences.getBool("dhcp", true);
  String ip = preferences.getString("ip", "192.168.1.10");
  String gateway = preferences.getString("gateway", "192.168.1.1");
  String subnet = preferences.getString("subnet", "255.255.0.0");
  String hostname = preferences.getString("hostname", "trance");
  preferences.end();
  
  // Apply Network Preferences
  WiFi.setHostname(hostname.c_str());

  // Set Static ip if DHCP is disabled
  if (!dhcp) {
    WiFi.config(
      IPAddress().fromString(ip),
      IPAddress().fromString(gateway),
      IPAddress().fromString(subnet)
    );
  } 
}

// Load WiFi Preferences and start 
void start_wifi() {
  // Loading wifi preferences
  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", "t"); 
  String password = preferences.getString("password", "t");
  preferences.end();

  // End previous Connection attemps
  WiFi.disconnect(true);

  // Add Listeners to wifi connection Events
  WiFi.onEvent(wifi_connected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(wifi_fail, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(ap_start, ARDUINO_EVENT_WIFI_AP_START);
  
  // Start to connect with these Credentials
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
}

// Open a AP when wifi connection is interrupted.
void wifi_fail(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("WIFI: Not able to connect, switching to AP STA mode");
  WiFi.softAP("trance-config");
  WiFi.mode(WIFI_AP_STA);
}

// Close AP when connection is successfull
void wifi_connected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("WIFI: Connected to Wifi, siwtching to STA mode");
  WiFi.mode(WIFI_STA);
}

void ap_start(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("WIFI: Soft AP started, Starting DNS");
  //DEBUG_PRINTLN("WIFI: Soft AP IP is: %s", WiFi.softAPIP());
  dnsServer.start(53, "*", WiFi.softAPIP());
}

void loop() {
  // Handle DNS requests for the Captive Portal
  dnsServer.processNextRequest();
}

// Device Reset
void reset() {
  // Remove all Preferences
  preferences.begin("wifi", false);
  preferences.clear();
  preferences.end();
}
