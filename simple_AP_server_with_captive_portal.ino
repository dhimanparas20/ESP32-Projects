#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#define LED_PIN 2  // Built-in LED

// AP credentials
const char* ap_ssid = "ESP32_LED_AP";
const char* ap_password = "12345678";

// Web server on port 80
WebServer server(80);

// DNS server for captive portal
DNSServer dnsServer;
const byte DNS_PORT = 53;

// HTML page with LED control buttons
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED Control</title>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: Arial; text-align: center; background: #101010; color: #fff; }
    h1 { margin-top: 30px; }
    button { padding: 15px 25px; font-size: 20px; margin: 10px; border: none; border-radius: 10px; cursor: pointer; }
    .on { background: #4CAF50; }
    .off { background: #E53935; }
  </style>
</head>
<body>
  <h1>ESP32 LED Control</h1>
  <p>LED Status: <span id="status">UNKNOWN</span></p>
  <button class="on" onclick="fetch('/led/on')">Turn ON</button>
  <button class="off" onclick="fetch('/led/off')">Turn OFF</button>

  <script>
    async function updateStatus() {
      const res = await fetch('/led/status');
      document.getElementById('status').innerText = res.ok ? await res.text() : 'ERROR';
    }
    setInterval(updateStatus, 1000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";

// LED state
bool ledState = false;

// Web server handlers
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleLedOn() {
  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/plain", "ON");
  Serial.println("LED turned ON via AP");
}

void handleLedOff() {
  ledState = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "OFF");
  Serial.println("LED turned OFF via AP");
}

void handleLedStatus() {
  server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Start Wi-Fi AP
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP Started! Connect to: ");
  Serial.println(ap_ssid);
  Serial.print("IP address: ");
  Serial.println(apIP);

  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", apIP);

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/led/status", handleLedStatus);

  server.begin();
  Serial.println("Web server started on port 80");
}

void loop() {
  // Handle web server clients
  server.handleClient();

  // Handle DNS server (redirect all requests to AP IP)
  dnsServer.processNextRequest();
}
