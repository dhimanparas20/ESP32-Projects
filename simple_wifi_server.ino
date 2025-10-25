#include <WiFi.h>

// ⚙️ Replace with your Wi-Fi credentials
const char* ssid     = "";
const char* password = "";

#define LED_PIN 2  // Built-in LED pin (usually GPIO 2)

WiFiServer server(80);
String header;      // To store HTTP request
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {
    Serial.println("New Client Connected.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          // If a blank line is received, the request is complete
          if (currentLine.length() == 0) {
            // Handle LED commands
            if (header.indexOf("GET /led/on") >= 0) {
              ledState = true;
              digitalWrite(LED_PIN, HIGH);
              Serial.println("LED turned ON via web");
            } else if (header.indexOf("GET /led/off") >= 0) {
              ledState = false;
              digitalWrite(LED_PIN, LOW);
              Serial.println("LED turned OFF via web");
            }

            // Build HTML response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<style>body{font-family:Arial;text-align:center;background:#101010;color:#fff;}button{padding:15px 25px;font-size:20px;margin:10px;border:none;border-radius:10px;cursor:pointer;} .on{background:#4CAF50;} .off{background:#E53935;}</style>");
            client.println("</head><body>");
            client.println("<h1>ESP32 LED Control</h1>");

            if (ledState) {
              client.println("<p>LED is <strong>ON</strong></p>");
              client.println("<a href='/led/off'><button class='off'>Turn OFF</button></a>");
            } else {
              client.println("<p>LED is <strong>OFF</strong></p>");
              client.println("<a href='/led/on'><button class='on'>Turn ON</button></a>");
            }

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
