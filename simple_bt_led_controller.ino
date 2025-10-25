#include "BluetoothSerial.h"

#define LED_PIN 2  // Built-in LED pin

BluetoothSerial SerialBT;  // Create Bluetooth Serial object

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Starting Bluetooth...");
  SerialBT.begin("MST BT"); // Bluetooth device name
  Serial.println("Bluetooth started! Pair with this name.");
}

void loop() {
  // If data is received via Bluetooth
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n'); // Read message until newline
    received.trim();  // Remove whitespace or \r

    Serial.print("Received via Bluetooth: ");
    Serial.println(received);

    if (received.equalsIgnoreCase("ON")) {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("LED turned ON");
      Serial.println("LED turned ON");
    } 
    else if (received.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("LED turned OFF");
      Serial.println("LED turned OFF");
    } 
    else {
      SerialBT.println("Unknown command. Send 'ON' or 'OFF'.");
    }
  }
}
