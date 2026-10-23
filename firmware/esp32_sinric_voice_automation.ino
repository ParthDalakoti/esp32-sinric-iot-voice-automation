#include <Arduino.h> 
#include <WiFi.h> 
#include "SinricPro.h" 
Ly WIFI_SSID   "PARTH" 
#define WIFI_PASS   "parth2005" 
 
// ======== SINRIC ======== 
#define APP_KEY     "01b982f3-b0bb-4e8e-b338-b3b72b3be321" 
#define APP_SECRET  
"ccb6e363-0f7a-40eb-b1c0-cf479c1d5f12-812691f6-20f1-4cf3-be52-c269ab21dfcb" 
 
// ======== DEVICE IDs ======== 
#define LED_DEVICE_ID   "6950085f00f870dd77d68607" 
#define FAN_DEVICE_ID   "6950178e729a4887d7eb947d" 
 
// ======== PINS ======== 
#define LED_PIN         26 
#define FAN_RELAY_PIN   27      // Relay module yahan connected hai 
#define WIFI_LED        2 
 
#define BAUD_RATE       9600 
 
// LED Callback (Direct LED - Active HIGH) 
bool onPowerState_LED(const String &deviceId, bool &state) { 
  Serial.print("[LED] Requested: "); 
  Serial.println(state ? "ON" : "OFF"); 
   
  digitalWrite(LED_PIN, state ? HIGH : LOW); 
  return true; 
} 
 
// Fan Callback with Relay (Most Common - Active LOW) 
bool onPowerState_FAN(const String &deviceId, bool &state) { 
  Serial.printf("[FAN] %s\r\n", state ? "ON" : "OFF"); 
 
  if (state) { 
    digitalWrite(FAN_RELAY_PIN, LOW);   // Relay ON 
    Serial.println("[FAN] Relay ON (LOW)"); 
  } else { 
    digitalWrite(FAN_RELAY_PIN, HIGH);  // Relay OFF 
    Serial.println("[FAN] Relay OFF (HIGH)"); 
  } 
  return true; 
} 

 
// WiFi Setup 
void setupWiFi() { 
  WiFi.begin(WIFI_SSID, WIFI_PASS); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println("\nWiFi Connected!"); 
  digitalWrite(WIFI_LED, LOW); 
} 
 
// Sinric Setup 
void setupSinric() { 
  SinricProSwitch &led = SinricPro[LED_DEVICE_ID]; 
  led.onPowerState(onPowerState_LED); 
 
  SinricProSwitch &fan = SinricPro[FAN_DEVICE_ID]; 
  fan.onPowerState(onPowerState_FAN); 
 
  SinricPro.begin(APP_KEY, APP_SECRET); 
  SinricPro.restoreDeviceStates(true); 
} 
 
// Setup 
void setup() { 
  Serial.begin(BAUD_RATE); 
  Serial.println("\n=== LED + Fan (Relay) Starting ===\n"); 
 
  // Pin Setup 
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, LOW);           // LED OFF at start 
 
  pinMode(FAN_RELAY_PIN, OUTPUT); 
  digitalWrite(FAN_RELAY_PIN, HIGH);    // Relay OFF at start (Active LOW) 
 
  pinMode(WIFI_LED, OUTPUT); 
  digitalWrite(WIFI_LED, HIGH); 
 
  setupWiFi(); 
  setupSinric(); 
 
  Serial.println("=== System Ready ==="); 
  Serial.println("Google Assistant se test kar sakte ho:"); 
  Serial.println("- 'Turn on LED'"); 
  Serial.println("- 'Turn on Fan'"); 
} 
 

// Loop 
void loop() { 
  SinricPro.handle(); 
  delay(5); 
}