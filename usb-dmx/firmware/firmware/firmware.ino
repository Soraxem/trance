/*

Firmware for the Trance hardware.

Prequisites:
Boards: esp32, 3.2.1
Librarys: 

Upload Settings:
Board: ESP32-S3-USB-OTG
Upload Mode: UART0 / Hardware CDC
USB Mode: USB-OTG

Author: Samuel Hafen

*/

// Comment or uncomment this line to enable/disable debugging
//#define DEBUG
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

// Device initialisation
void setup() {

  // Activate serial communication when debugging is enabled
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  // Loading wifi preferences
  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", ""); 
  String password = preferences.getString("password", "");
  preferences.end();

  // Add Listeners to wifi connection Events
  WiFi.onEvent(wifi_connected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(wifi_disconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  // Start to connect with these Credentials
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  // TODO: Loading Network Preferences
  WiFi.setHostname("trance");

}

// Open a AP when wifi connection is interrupted.
void wifi_disconnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("WIFI: Not able to connect, switching to AP STA mode");
  WiFi.mode(WIFI_AP_STA);
}

// Close AP when connection is successfull
void wifi_connected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("WIFI: Connected to Wifi, siwtching to STA mode");
  WiFi.mode(WIFI_STA);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

// Device Reset
void reset() {
  // Remove all Preferences
  preferences.begin("wifi", false);
  preferences.clear();
  preferences.end();
}
