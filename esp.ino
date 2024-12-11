#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

const char* ssid = "Amit";
const char* password = "12345678";

const char* serverUrl = "http://192.168.80.200:5000/data";

#define ECG_PIN 35
#define DIGITAL_PIN_1 2
#define DIGITAL_PIN_2 4

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  int maxAttempts = 20;
  while (WiFi.status() != WL_CONNECTED && maxAttempts > 0) {
    delay(1000);
    Serial.println("Still trying to connect...");
    maxAttempts--;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi");
  } else {
    Serial.println("Failed to connect to Wi-Fi");
    return;
  }

  if (!mpu.begin()) {
    Serial.println("MPU6050 initialization failed!");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(DIGITAL_PIN_1, INPUT);
  pinMode(DIGITAL_PIN_2, INPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    bool digital1 = digitalRead(DIGITAL_PIN_1);
    bool digital2 = digitalRead(DIGITAL_PIN_2);
    int ecg_value = analogRead(ECG_PIN);

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    String payload = "{";
    payload += "\"digital1\":" + String(digital1) + ",";
    payload += "\"digital2\":" + String(digital2) + ",";
    payload += "\"ecg\":" + String(ecg_value) + ",";
    payload += "\"acc_x\":" + String(a.acceleration.x) + ",";
    payload += "\"acc_y\":" + String(a.acceleration.y) + ",";
    payload += "\"acc_z\":" + String(a.acceleration.z) + ",";
    payload += "\"gyro_x\":" + String(g.gyro.x) + ",";
    payload += "\"gyro_y\":" + String(g.gyro.y) + ",";
    payload += "\"gyro_z\":" + String(g.gyro.z) + ",";
    payload += "\"temp\":" + String(temp.temperature);
    payload += "}";

    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error in sending data");
    }

    http.end();
  } else {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
  }

  delay(500);
}
