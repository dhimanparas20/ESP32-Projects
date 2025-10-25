#include <WiFi.h>

const char* ssid     = "MST Protocol";
const char* password = "Blackkaiser69";

#define LED_PIN 2  // Built-in LED pin (usually GPIO 2)
unsigned long lastCheck = 0; // Timer for status printing

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("📶 Starting Wi-Fi connection...");
  WiFi.begin(ssid, password);
}

void loop() {
  // If not connected
  if (WiFi.status() != WL_CONNECTED) {
    // WiFi.reconnect();
    Serial.println("⏳ Connecting to Wi-Fi...");
    // Blink LED while connecting
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  } 
  else {
    // Connected to Wi-Fi
    digitalWrite(LED_PIN, HIGH);  // Keep LED ON

    // Print connection info every 5 seconds
    if (millis() - lastCheck > 5000) {
      lastCheck = millis();
      Serial.println("✅ Connected to Wi-Fi!");
      Serial.print("📡 SSID: ");
      Serial.println(WiFi.SSID());
      Serial.print("💻 IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("🔢 MAC Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("📶 Signal Strength (RSSI): ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      Serial.println("-----------------------------");
    }

    delay(100);  // Short delay before next loop iteration
  }
}
