// =========================================================================
// 🚀 ECOSENSE INTEGRATED INDUSTRIAL HARDWARE FIRMWARE WITH GITHUB OTA ENGINES
// =========================================================================

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h> // Automated smart-camera style captive provisioning portal
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// --- Physical Sensor Pin Definitions ---
#define DHTPIN 2       // GPIO2 maps exactly to physical pin D4 on the NodeMCU board layout
#define DHTTYPE DHT22  // Ambiance tracking sensor type
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;   // Hardware I2C pins: SCL=D1 (GPIO5), SDA=D2 (GPIO4)

// --- Cloud Gateway Storage Buffers (Increased to 100 to fix truncation faults) ---
char google_script_id[100] = "AKfycbyHOsDvVszXSYMZzQ5HVoPB7NkBneemslbv8ODdbhRrzS1jHKhtpxhwQh_ZjgilY4UD";
const char* host = "script.google.com";

// 🚀 PASTE YOUR DIRECT GITHUB REPOSITORY RAW BINARY PATH STRING HERE:
const String bin_file_url = "https://raw.githubusercontent.com/Ecosense-web/ecosense-firmware/main/firmware.bin";

// --- Time Delays Tracker ---
unsigned long lastExecutionTime = 0;
const unsigned long checkingInterval = 300000; // 5-Minute Evaluation Interval Loop Cycles (300,000 ms)

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ECOSENSE SMART HARDWARE PLATFORM INITIALIZING ===");
  
  dht.begin();
  
  if (!bmp.begin(0x76)) {
    Serial.println("⚠️ Warning: BMP280 atmospheric metric array missing or detached!");
  }

  // --- WiFiManager Captive Setup Loop Sequence ---
  WiFiManager wifiManager;
  
  // Custom Parameter setup to pass the Web App Gateway ID cleanly via internal memory blocks
  WiFiManagerParameter custom_google_id("script_id", "Cloud Secret Gateway ID", google_script_id, 100);
  wifiManager.addParameter(&custom_google_id);
  
  // Launches local configuration hotspot "Econode Smart Device" if tracking connection parameters are lost
  if (!wifiManager.autoConnect("Econode Smart Device")) {
    Serial.println("Connection timeout triggered. Performing hardware warm restart...");
    delay(3000);
    ESP.restart();
  }

  // Reload the parameters confirmed inside the user setup interface
  strcpy(google_script_id, custom_google_id.getValue());
  Serial.println("🌐 Local network connection established cleanly!");
  Serial.print("Active Google Cloud Secret Token Pointer: ");
  Serial.println(google_script_id);
  
  // Fire an immediate startup packet post to clear the pipeline registers instantly
  executeCloudTelemetryHandshake();
}

void loop() {
  // Execute a clean sensor dump loop precisely every 5 minutes without using blocking delay chains
  if (millis() - lastExecutionTime >= checkingInterval) {
    lastExecutionTime = millis();
    
    if (WiFi.status() == WL_CONNECTED) {
      executeCloudTelemetryHandshake();
    } else {
      Serial.println("❌ Network disconnected! Re-evaluating routing traces...");
    }
  }
}

// --- Unified Data Handshake and Remote Command Parsing Loop ---
void executeCloudTelemetryHandshake() {
  WiFiClientSecure client;
  client.setInsecure(); // Allows dynamic bypass processing on shifting Google proxy load structures
  
  HTTPClient http;
  
  // Read local edge sensor matrices
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float p = bmp.readPressure() / 100.0F; // Convert to standard millibars/hPa
  int a = analogRead(A0);                 // Pull analog toxicity metrics directly from the MQ heater core
  
  // Edge protection checking parameters
  if (isnan(t) || isnan(h)) {
    Serial.println("⚠️ Error: DHT22 metrics returned empty arrays. Checking core logic line wiring traces...");
    t = 0.00;
    h = 0.00;
  }

  // Build the complete URL query tracking parameters matching our Google Apps Script parsing keys
  String url = "https://" + String(host) + "/macros/s/" + String(google_script_id) + "/exec" +
               "?temp=" + String(t) + "&hum=" + String(h) + "&press=" + String(p) + "&air=" + String(a);
               
  Serial.println("\n📡 Streaming current sensor matrices to cloud analytical vaults...");
  
  if (http.begin(client, url)) {
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String serverResponse = http.getString();
      Serial.print("Cloud Feedback Signal Verified: ");
      Serial.println(serverResponse);
      
      // --- Integrated Remote Instruction Matrix Execution Engine ---
      if (serverResponse.indexOf("CMD_EXECUTE:REBOOT_NODE") >= 0) {
        Serial.println("🚨 REMOTE OVERRIDE: Warm reboot command validated! Purging active memory maps...");
        delay(1000);
        ESP.restart(); // Software execution trigger to cycle the microcontroller core immediately
      }
      else if (serverResponse.indexOf("CMD_EXECUTE:UPDATE_FIRMWARE") >= 0) {
        Serial.println("📥 REMOTE OVERRIDE: Activating Over-The-Air Online Update sequence...");
        http.end(); // Clear socket stream allocations to optimize heap space before flash updates open
        executeOTAUpdateRoutine();
      }
    } else {
      Serial.printf("Gateway connection error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

// --- Independent High-Priority GitHub OTA Storage File Streamer ---
void executeOTAUpdateRoutine() {
  WiFiClientSecure otaClient;
  otaClient.setInsecure(); // Drops strict certificate verification to stream raw binaries smoothly
  
  Serial.println("🔄 Establishing secure socket link to GitHub CDN servers...");
  
  // Configure internal hardware diagnostic LED to stay turned ON solidly during the download
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  
  // Force the update routine to strictly resolve and follow HTTP 302 redirects (Crucial for GitHub architecture)
  ESPhttpUpdate.followRedirects(true);

  Serial.println("📥 Downloading binary payload stream and rewriting flash data sectors...");
  t_httpUpdate_return ret = ESPhttpUpdate.update(otaClient, bin_file_url);
  
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("❌ GitHub OTA Script Refused! Error code (%d): %s\n", 
                    ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
      
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("💤 Server message code: Flash matching signature is identical. No changes deployed.");
      break;
      
    case HTTP_UPDATE_OK:
      Serial.println("🌟 Success! Entire codebase flashed cleanly. Rebooting node layout...");
      // Board handles a structural warm boot into your brand new codebase here.
      break;
  }
}