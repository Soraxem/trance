/*

Firmware for the Trance hardware.

Prequisites:
Boards: esp32 v3.2.1
Librarys: ESPAsyncWebServer v3.7.10, AsyncTCP v3.4.5, Preferences v2.1.0, ESPAsyncE1.31 v1.0.3, FastLED v3.10.2

Upload Settings:
Board: ESP32-S3-USB-OTG
Upload Mode: UART0 / Hardware CDC
USB Mode: Hardware CDC and JTAG

Author: Samuel Hafen

*/


// Comment or uncomment this line to enable/disable debugging§
#define DEBUG
// Create macros for serial debugging
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Define macros for configuration
#define WIFI 1
#define ETHERNET 2

#include <FastLED.h>

// Import Hardware definition
// Uncomment the hardware you are using
#include <rgb-c3.h>
//#include <pixel-c3.h>

// Allows for persisten Preferences storage
#include <Preferences.h>
Preferences preferences;

// Resources for Captive Portal and Configuration
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
static DNSServer dnsServer;
static AsyncWebServer server(80);

// Custom Webinterface
#include "webinterface.html"

// Load Modules
#include <networking.h>
#include <connections.h>

// Ascn Connectivity
#include <ESPAsyncE131.h>
#define UNIVERSE 1                      // First DMX Universe to listen for
#define UNIVERSE_COUNT 1                // Total number of Universes to listen for, starting at UNIVERSE
#define ADDRESS 15

// ESPAsyncE131 instance with UNIVERSE_COUNT buffer slots
ESPAsyncE131 e131(UNIVERSE_COUNT);

// Device initialisation
void setup() {

  interface_setup();

  // Activate serial communication when debugging is enabled
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000); // wait for Serial CDC to open
  #endif
  DEBUG_PRINTLN("-- Start Debugging --");

  load_network_preferences();
  network_setup();

  server.on("/", HTTP_GET, on_config);
  server.onNotFound(onRequest);
  server.begin();

  analogWrite(2, 10);

  connection_setup();

}

// Redirect if request path is unknown
void onRequest(AsyncWebServerRequest *request){
  request->redirect("http://" + WiFi.softAPIP().toString() );
}

void on_config(AsyncWebServerRequest *request){

  // Handle wifi Configuration
  if(request->hasParam("ssid") && request->hasParam("password")) {
    
    // Read values from url Parameters
    String ssid = request->getParam("ssid")->value();
    String password = request->getParam("password")->value();

    // Save Values to Preferences
    preferences.begin("wifi");
    preferences.putString("ssid", ssid); 
    preferences.putString("password", password);
    preferences.end();
    DEBUG_PRINTLN("CONFIG: recieved wifi configuration");

    // Reset Wifi connection and try to ceonnect
    start_wifi();
  }

  // Handle Connectivity Configuration
  if(request->hasParam("connections")) {
    
    // Read value from url Parameters
    String connections = request->getParam("connections")->value();

    // Save Values to Preferences
    preferences.begin("connectivity");
    preferences.putString("connections", connections);
    preferences.end();
    DEBUG_PRINTLN("CONFIG: Recieved Connectivity configuration");
  }

  // Display the webinterface
  request->send(200, "text/html", webinterface, processor);
}

// Replaces placeholder with button section in your web page
String processor(const String& var){

  // Loading wifi preferences
  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", "t"); 
  String password = preferences.getString("password", "t");
  preferences.end();

  // Return wifi Settings
  if(var == "ssid") return ssid;
  if(var == "password") return password;

  // Loading Network Preferences
  preferences.begin("network", true);
  bool dhcp = preferences.getBool("dhcp", true);
  String ip = preferences.getString("ip", "192.168.1.10");
  String gateway = preferences.getString("gateway", "192.168.1.1");
  String subnet = preferences.getString("subnet", "255.255.0.0");
  String hostname = preferences.getString("hostname", "trance");
  preferences.end();

  // Return Network Settings
  if(var == "dhcp") {
    if(dhcp) return "checked";
    if(!dhcp) return "";
  }
  if(var == "fixed") {
    if(!dhcp) return "checked";
    if(dhcp) return "";
  } 
  if(var == "ip") return ip;
  if(var == "gateway") return gateway;
  if(var == "subnet") return subnet;
  if(var == "hostname") return hostname;

  // Load Connectivity Preferences
  preferences.begin("connectivity", true);
  String connections = preferences.getString("connections", "");
  preferences.end();
  
  // return Connectivity Settings
  if(var == "ascn" && connections == "ascn") {
    return "checked";
  }
  if(var == "artnet" && connections == "artnet") {
    return "checked";
  }
  if(var == "mqtt" && connections == "mqtt") {
    return "checked";
  }
 
  return String();
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

void loop() {
  network_handle();
  interface_handle();
  connection_handle();
}

// Device Reset
void reset() {
  // Remove all Preferences
  preferences.begin("wifi", false);
  preferences.clear();
  preferences.end();
}
