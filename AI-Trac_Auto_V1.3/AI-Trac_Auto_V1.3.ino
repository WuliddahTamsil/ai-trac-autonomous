/*
 * AI-TRAC AUTONOMOUS RC CAR V1.2.2 - SIMPLIFIED VERSION
 * RnD IPB - Fixed WiFi and Waypoints Issues
 */

// ========== LIBRARIES ==========
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "webpage_v2.h"

// ========== PIN DEFINITIONS ==========
// Motor Control
#define MOTOR_R_PWM 13
#define MOTOR_R_EN 12
#define MOTOR_L_PWM 14
#define MOTOR_L_EN 26

// PWM Channels
#define PWM_CH_LEFT 0
#define PWM_CH_RIGHT 1
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// Ultrasonic Sensors
#define US_FRONT_TRIG 32
#define US_FRONT_ECHO 33
#define US_LEFT_TRIG 21
#define US_LEFT_ECHO 22
#define US_RIGHT_TRIG 18
#define US_RIGHT_ECHO 19

// Status
#define LED_STATUS 2
#define BUZZER 15

// ========== GLOBAL OBJECTS ==========
WebServer server(80);

// ========== GLOBAL VARIABLES ==========
enum Mode { MANUAL, AUTO, LINE_FOLLOWING };
Mode currentMode = MANUAL;

// GPS & Time
float latitude = 0, longitude = 0;
float heading = 0, speed = 0;
int satellites = 0;
int timezoneOffset = 7;  // Default WIB (UTC+7)
bool useInternetTime = false;

// WiFi Configuration
String savedWiFiSSID = "";
String savedWiFiPass = "";
bool savedWiFiAutoConnect = false;
bool isAPMode = true;  // Default AP mode
bool isConnecting = false;

// Sensor Data
float usFront = 0, usLeft = 0, usRight = 0;
float altitude = 0;

// Control
int throttle = 0;
int steering = 0;

// Navigation
struct Waypoint {
  double lat;
  double lon;
  String name;
  bool reached;
};
std::vector<Waypoint> waypoints;

// Web Interface
unsigned long lastWebUpdate = 0;

// WiFi Scan Cache
struct WiFiNetwork {
  String ssid;
  int rssi;
  String bssid;
  uint8_t channel;
  uint8_t encType;
};
std::vector<WiFiNetwork> cachedNetworks;
unsigned long lastScanTime = 0;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n" + String(60, '='));
  Serial.println("AI-TRAC V1.2.2 - SIMPLIFIED");
  Serial.println("RnD IPB - Fixed Version");
  Serial.println(String(60, '='));
  
  // Initialize hardware
  initPins();
  Serial.println("[OK] GPIO Initialized");
  
  // Start WiFi AP Mode
  startAPMode();
  
  // Initialize Web Server
  initWebServer();
  
  Serial.println("\n" + String(60, '='));
  Serial.println("SYSTEM READY");
  Serial.print("Web Interface: http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("SSID: AI-TRAC");
  Serial.println("Password: 12345678");
  Serial.println(String(60, '='));
}

// ========== MAIN LOOP ==========
void loop() {
  static unsigned long lastSensorRead = 0;
  static unsigned long lastReconnectAttempt = 0;
  unsigned long currentTime = millis();
  
  if (WiFi.status() == WL_CONNECTED) isConnecting = false;
  // Auto-reconnect: jika STA putus, coba reconnect ke savedWiFiSSID
  if (WiFi.status() != WL_CONNECTED && !isConnecting && savedWiFiSSID.length() > 0) {
    if (currentTime - lastReconnectAttempt >= 10000) {
      lastReconnectAttempt = currentTime;
      Serial.println("[WiFi] Disconnected - attempting reconnect...");
      WiFi.disconnect();
      delay(200);
      WiFi.begin(savedWiFiSSID.c_str(), savedWiFiPass.c_str());
    }
  }
  
  // Read sensors every 100ms
  if (currentTime - lastSensorRead >= 100) {
    readSensors();
    lastSensorRead = currentTime;
  }
  
  // Handle web server
  server.handleClient();
  
  // Send web updates every 500ms
  if (currentTime - lastWebUpdate >= 500) {
    lastWebUpdate = currentTime;
  }
  
  // Safety checks
  safetyCheck();
  
  // Update status LED
  updateLEDStatus();
  
  // Simulate GPS data for testing
  simulateGPSData();
  
  delay(10);
}

// ========== WIFI FUNCTIONS ==========
void startAPMode() {
  Serial.println("[WiFi] Starting Access Point...");
  
  // Use AP+STA mode so scanning/connecting won't drop the AP
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("AI-TRAC", "12345678");

  isAPMode = true;
  isConnecting = false;

  delay(100);

  Serial.println("[WiFi] Access Point Active");
  Serial.print("[WiFi] SSID: AI-TRAC\n");
  Serial.print("[WiFi] SoftAP IP: "); Serial.println(WiFi.softAPIP());
  Serial.print("[WiFi] Station IP: "); Serial.println(WiFi.localIP());
  Serial.print("[WiFi] MAC: "); Serial.println(WiFi.softAPmacAddress());
}

void scanWiFiNetworks() {
  Serial.println("[WiFi] Scanning networks...");
  
  // Switch to STA mode for scanning
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  int n = WiFi.scanNetworks();
  Serial.printf("[WiFi] Found %d networks\n", n);
  
  // Return to AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP("AI-TRAC", "12345678");
}

// ========== SENSOR FUNCTIONS ==========
void readSensors() {
  usFront = readUltrasonic(US_FRONT_TRIG, US_FRONT_ECHO);
  usLeft = readUltrasonic(US_LEFT_TRIG, US_LEFT_ECHO);
  usRight = readUltrasonic(US_RIGHT_TRIG, US_RIGHT_ECHO);
}

float readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  return (duration == 0) ? 999.0 : duration * 0.0343 / 2.0;
}

void simulateGPSData() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    lastUpdate = millis();
    
    // Simulate GPS data
    latitude = -6.5 + (random(0, 100) / 10000.0);
    longitude = 106.8 + (random(0, 100) / 10000.0);
    speed = random(0, 50) / 10.0;
    heading = random(0, 360);
    satellites = random(5, 12);
  }
}

// ========== WEB SERVER FUNCTIONS ==========
void initWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/getData", HTTP_GET, handleGetData);
  server.on("/setMode", HTTP_GET, handleSetMode);
  server.on("/emergencyStop", HTTP_GET, handleEmergencyStop);
  server.on("/clearWaypoints", HTTP_GET, handleClearWaypoints);
  server.on("/startNavigation", HTTP_GET, handleStartNavigation);
  server.on("/centerMap", HTTP_GET, handleCenterMap);
  server.on("/setSteering", HTTP_GET, handleSetSteering);
  
  // WiFi management endpoints
  server.on("/api/wifi/scan", HTTP_GET, handleWiFiScan);
  server.on("/api/wifi/connect", HTTP_POST, handleWiFiConnect);
  server.on("/api/wifi/disconnect", HTTP_POST, handleWiFiDisconnect);
  server.on("/api/wifi/config", HTTP_GET, handleWiFiConfig);
  server.on("/api/wifi/toggleAP", HTTP_POST, handleWiFiToggleAP);
  
  // Time endpoints
  server.on("/api/time", HTTP_GET, handleTime);
  server.on("/api/time/gps", HTTP_GET, handleGpsTime);
  
  server.begin();
  Serial.println("[HTTP] Server started on port 80");
}

void handleRoot() {
  Serial.println("[HTTP] Root request received");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent_P(WEBPAGE_HTML);
  Serial.println("[HTTP] Root response sent");
}

// Get data for webpage
void handleGetData() {
  StaticJsonDocument<512> doc;
  
  // Vehicle data
  doc["speed"] = String(speed, 1);
  doc["heading"] = String(heading, 1);
  doc["gpsSats"] = satellites;
  doc["altitude"] = String(altitude, 1);
  
  // Control data
  doc["throttle"] = throttle;
  doc["steering"] = steering;
  
  // Ultrasonic sensors
  doc["usFront"] = usFront;
  doc["usLeft"] = usLeft;
  doc["usRight"] = usRight;
  
  // Mode
  doc["mode"] = (currentMode == MANUAL) ? "MANUAL" : 
                (currentMode == AUTO) ? "AUTO" : "LINE";
  
  // WiFi status: STA terhubung atau AP aktif
  doc["isAPMode"] = isAPMode;
  doc["isConnecting"] = isConnecting;
  bool staConnected = (WiFi.status() == WL_CONNECTED);
  doc["wifiConnected"] = isAPMode ? true : staConnected;
  
  if (isAPMode) {
    doc["wifiSSID"] = "AI-TRAC";
    doc["wifiIP"] = WiFi.softAPIP().toString();
    doc["wifiSignal"] = "AP Mode";
    doc["wifiMAC"] = WiFi.softAPmacAddress();
  } else {
    doc["wifiSSID"] = staConnected ? WiFi.SSID() : "Disconnected";
    doc["wifiIP"] = staConnected ? WiFi.localIP().toString() : "0.0.0.0";
    doc["wifiSignal"] = staConnected ? String(WiFi.RSSI()) + " dBm" : "No Signal";
    doc["wifiMAC"] = WiFi.macAddress();
  }
  
  // Time source - always GPS for now
  doc["timeSource"] = "GPS";
  
  // Waypoint count
  doc["waypointCount"] = waypoints.size();
  
  // Current waypoint index
  int currentWp = 0;
  for (size_t i = 0; i < waypoints.size(); i++) {
    if (!waypoints[i].reached) {
      currentWp = i + 1;
      break;
    }
  }
  doc["currentWaypoint"] = currentWp;
  
  // Current time (simulated)
  char timeStr[9];
  unsigned long seconds = millis() / 1000;
  int hour = (seconds / 3600 + timezoneOffset) % 24;
  int minute = (seconds % 3600) / 60;
  int second = seconds % 60;
  sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
  doc["currentTime"] = String(timeStr);
  
  doc["timezone"] = timezoneOffset;
  doc["timeSynced"] = true;
  
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// WiFi Scan - Aggressive multi-channel scan
void handleWiFiScan() {
  if (server.method() == HTTP_GET) {
    Serial.println("[WiFi] API: scan requested - Starting aggressive scan...");
    Serial.flush();
    
    cachedNetworks.clear();
    
    // Perform multiple scan attempts to catch networks on different channels
    // Try both standard and aggressive settings
    
    Serial.println("[WiFi] Attempt 1: Standard scan (all channels, show hidden)...");
    int n1 = WiFi.scanNetworks(false, true);  // non-async, show hidden networks
    
    Serial.printf("[WiFi] Attempt 1 found: %d networks\n", n1);
    unsigned long scanTime = millis();
    
    // Store results from first scan
    std::vector<String> foundSSIDs;
    if (n1 > 0) {
      for (int i = 0; i < n1 && i < 50; i++) {
        String ssid = WiFi.SSID(i);
        if (ssid.length() > 0) {
          foundSSIDs.push_back(ssid);
        }
      }
    }
    
    // Delay between scans to let radio settle
    delay(500);
    
    // Try second scan with different parameters (might catch different APs)
    Serial.println("[WiFi] Attempt 2: Second scan to catch different APs...");
    int n2 = WiFi.scanNetworks(false, true);
    
    Serial.printf("[WiFi] Attempt 2 found: %d networks\n", n2);
    
    // Combine results from both scans
    for (int i = 0; i < n2 && i < 50; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      
      // Skip empty SSIDs
      if (ssid.length() == 0) {
        Serial.printf("[WiFi] Skipping empty SSID at index %d\n", i);
        continue;
      }
      
      // Check if SSID already in cache (avoid duplicates)
      bool isDuplicate = false;
      for (const auto& net : cachedNetworks) {
        if (net.ssid == ssid) {
          isDuplicate = true;
          // Update with stronger signal if this one is better
          if (rssi > net.rssi) {
            for (auto& existing : cachedNetworks) {
              if (existing.ssid == ssid) {
                existing.rssi = rssi;
                existing.channel = WiFi.channel(i);
                break;
              }
            }
          }
          break;
        }
      }
      
      if (!isDuplicate) {
        WiFiNetwork net;
        net.ssid = ssid;
        net.rssi = rssi;
        net.bssid = WiFi.BSSIDstr(i);
        net.channel = WiFi.channel(i);
        net.encType = WiFi.encryptionType(i);
        
        cachedNetworks.push_back(net);
        Serial.printf("[WiFi] CACHED: SSID: %s | RSSI: %d | CH: %d | BSSID: %s | ENC: %d\n", 
                      net.ssid.c_str(), net.rssi, net.channel, net.bssid.c_str(), net.encType);
      }
    }
    
    scanTime = millis() - scanTime;
    Serial.printf("[WiFi] Scan complete! Total unique networks: %d | Time: %lu ms\n", 
                  (int)cachedNetworks.size(), scanTime);
    
    lastScanTime = millis();
    
    // Prepare response
    StaticJsonDocument<8192> doc;
    JsonArray networks = doc.createNestedArray("networks");
    
    // Sort by RSSI (strongest first)
    std::sort(cachedNetworks.begin(), cachedNetworks.end(),
              [](const WiFiNetwork& a, const WiFiNetwork& b) { return a.rssi > b.rssi; });
    
    for (const auto& net : cachedNetworks) {
      JsonObject network = networks.createNestedObject();
      network["ssid"] = net.ssid;
      network["rssi"] = net.rssi;
      network["bssid"] = net.bssid;
      network["channel"] = net.channel;
      network["security"] = (net.encType == WIFI_AUTH_OPEN) ? "Open" : "Encrypted";
      
      // Calculate signal strength percentage (0-100)
      int strength = 0;
      if (net.rssi >= -50) strength = 100;
      else if (net.rssi < -100) strength = 0;
      else strength = 2 * (net.rssi + 100);
      
      network["strength"] = String(net.rssi) + " dBm";
      network["quality"] = strength;
    }
    
    doc["count"] = cachedNetworks.size();
    doc["scanning"] = false;
    doc["apMode"] = isAPMode;
    doc["timestamp"] = millis();
    doc["scanTime"] = scanTime;
    
    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
    
    Serial.printf("[WiFi] Response sent! Total: %d networks\n", cachedNetworks.size());
    Serial.flush();
  }
}


// WiFi Connect
void handleWiFiConnect() {
  if (server.method() == HTTP_POST) {
    String body = server.arg("plain");
    StaticJsonDocument<512> doc;

    DeserializationError err = deserializeJson(doc, body);
    if (err) {
      StaticJsonDocument<128> errDoc;
      errDoc["ok"] = false;
      errDoc["error"] = "Invalid JSON";
      String errJson;
      serializeJson(errDoc, errJson);
      server.send(400, "application/json", errJson);
      return;
    }
    const char* ssid = doc["ssid"];
    const char* password = doc["password"] | "";
    if (!ssid || strlen(ssid) == 0) {
      StaticJsonDocument<128> errDoc;
      errDoc["ok"] = false;
      errDoc["error"] = "SSID required";
      String errJson;
      serializeJson(errDoc, errJson);
      server.send(400, "application/json", errJson);
      return;
    }
    savedWiFiSSID = String(ssid);
    savedWiFiPass = String(password);
    isConnecting = true;
    WiFi.begin(ssid, password);
    StaticJsonDocument<128> respDoc;
    respDoc["ok"] = true;
    respDoc["message"] = "Connecting";
    String respJson;
    serializeJson(respDoc, respJson);
    server.send(200, "application/json", respJson);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleWiFiDisconnect() {
  WiFi.disconnect();
  delay(100);
  savedWiFiSSID = "";
  savedWiFiPass = "";
  StaticJsonDocument<64> doc;
  doc["ok"] = true;
  doc["message"] = "Disconnected";
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleWiFiConfig() {
  StaticJsonDocument<256> doc;
  doc["ssid"] = savedWiFiSSID;
  doc["apMode"] = isAPMode;
  doc["autoConnect"] = savedWiFiAutoConnect;
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleWiFiToggleAP() {
  isAPMode = !isAPMode;
  StaticJsonDocument<64> doc;
  doc["apMode"] = isAPMode;
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleTime() {
  unsigned long totalSeconds = (millis() / 1000) + (timezoneOffset * 3600);
  int seconds = totalSeconds % 86400;
  if (seconds < 0) seconds += 86400;
  int hour = seconds / 3600;
  int minute = (seconds % 3600) / 60;
  int second = seconds % 60;
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
  StaticJsonDocument<128> doc;
  doc["time"] = String(timeStr);
  doc["timezone"] = timezoneOffset;
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleGpsTime() {
  unsigned long totalSeconds = (millis() / 1000) + (timezoneOffset * 3600);
  int seconds = totalSeconds % 86400;
  if (seconds < 0) seconds += 86400;
  int hour = seconds / 3600;
  int minute = (seconds % 3600) / 60;
  int second = seconds % 60;
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
  StaticJsonDocument<384> doc;
  doc["time"] = String(timeStr);
  doc["timezone"] = timezoneOffset;
  doc["fix"] = (satellites > 0);
  doc["sats"] = satellites;
  doc["lat"] = String(latitude, 6);
  doc["lon"] = String(longitude, 6);
  doc["latitude"] = String(latitude, 6);
  doc["longitude"] = String(longitude, 6);
  doc["satellites"] = satellites;
  doc["altitude"] = String(altitude, 1);
  doc["heading"] = String(heading, 1);
  doc["date"] = "2024-01-01";
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// Set mode
void handleSetMode() {
  String mode = server.arg("mode");
  
  if (mode == "manual") {
    currentMode = MANUAL;
    Serial.println("[WEB] Mode: MANUAL");
  } else if (mode == "auto") {
    currentMode = AUTO;
    Serial.println("[WEB] Mode: AUTO");
  } else if (mode == "line") {
    currentMode = LINE_FOLLOWING;
    Serial.println("[WEB] Mode: LINE");
  }
  
  server.send(200, "text/plain", "Mode: " + mode);
}

// Emergency stop
void handleEmergencyStop() {
  throttle = 0;
  steering = 0;
  currentMode = MANUAL;
  
  // Stop motors immediately
  setMotorSpeed(0, 0);
  setMotorSpeed(1, 0);
  
  Serial.println("[EMERGENCY] Stop activated!");
  server.send(200, "text/plain", "EMERGENCY STOP");
}

// Clear waypoints
void handleClearWaypoints() {
  waypoints.clear();
  Serial.println("[WEB] Waypoints cleared");
  server.send(200, "text/plain", "Waypoints cleared");
}

// Start navigation
void handleStartNavigation() {
  if (waypoints.empty()) {
    server.send(400, "text/plain", "No waypoints");
    return;
  }
  
  currentMode = AUTO;
  Serial.println("[WEB] Navigation started");
  server.send(200, "text/plain", "Navigation started");
}

// Center map
void handleCenterMap() {
  Serial.println("[WEB] Map centered");
  server.send(200, "text/plain", "Map centered");
}

// Set steering
void handleSetSteering() {
  String value = server.arg("value");
  steering = value.toInt();
  Serial.printf("[WEB] Steering: %d\n", steering);
  server.send(200, "text/plain", "Steering: " + value);
}

// ========== MOTOR CONTROL ==========
void setMotorSpeed(int motor, int speed) {
  int pwmPin, enPin, pwmChannel;
  
  if (motor == 0) {
    pwmPin = MOTOR_L_PWM;
    enPin = MOTOR_L_EN;
    pwmChannel = PWM_CH_LEFT;
  } else {
    pwmPin = MOTOR_R_PWM;
    enPin = MOTOR_R_EN;
    pwmChannel = PWM_CH_RIGHT;
  }
  
  int pwmValue = map(abs(speed), 0, 100, 0, 255);
  
  if (speed > 0) {
    digitalWrite(enPin, HIGH);
    ledcWrite(pwmChannel, pwmValue);
  } else if (speed < 0) {
    digitalWrite(enPin, LOW);
    ledcWrite(pwmChannel, pwmValue);
  } else {
    digitalWrite(enPin, LOW);
    ledcWrite(pwmChannel, 0);
  }
}

// ========== HARDWARE FUNCTIONS ==========
void initPins() {
  // Setup PWM untuk motor
  ledcSetup(PWM_CH_LEFT, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CH_RIGHT, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_L_PWM, PWM_CH_LEFT);
  ledcAttachPin(MOTOR_R_PWM, PWM_CH_RIGHT);
  
  ledcWrite(PWM_CH_LEFT, 0);
  ledcWrite(PWM_CH_RIGHT, 0);
  
  // Motor enable pins
  pinMode(MOTOR_L_EN, OUTPUT);
  pinMode(MOTOR_R_EN, OUTPUT);
  digitalWrite(MOTOR_L_EN, LOW);
  digitalWrite(MOTOR_R_EN, LOW);
  
  // Ultrasonic pins
  pinMode(US_FRONT_TRIG, OUTPUT);
  pinMode(US_FRONT_ECHO, INPUT);
  pinMode(US_LEFT_TRIG, OUTPUT);
  pinMode(US_LEFT_ECHO, INPUT);
  pinMode(US_RIGHT_TRIG, OUTPUT);
  pinMode(US_RIGHT_ECHO, INPUT);
  
  // Status pins
  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(BUZZER, LOW);
  
  Serial.println("[OK] Pins initialized");
}

void safetyCheck() {
  if (usFront < 10.0) {
    throttle = 0;
    steering = 0;
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }
}

void updateLEDStatus() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink >= 1000) {
    ledState = !ledState;
    digitalWrite(LED_STATUS, ledState);
    lastBlink = millis();
  }
}
