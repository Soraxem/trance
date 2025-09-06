/*

Network managing module for the trance firmware

*/

#if TRANCE_WIFI == true

// Library for connecting to Wifi networks
#include <WiFi.h>

void network_setup() {
  start_wifi();
}

void network_handle() {
    // Handle DNS requests for the Captive Portal
  dnsServer.processNextRequest();
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


#endif