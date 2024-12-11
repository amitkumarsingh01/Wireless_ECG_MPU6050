#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.begin("Amit", "12345678");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Not connected to WiFi");
  }
  delay(1000);
}