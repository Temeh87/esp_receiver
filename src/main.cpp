#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OLED_SDA      21
#define OLED_SCL      22
#define OLED_I2C_ADDR 0x3C
#define OLED_WIDTH    128
#define OLED_HEIGHT   128

const unsigned long TIMEOUT_MS = 10000;

Adafruit_SH1107 display(OLED_WIDTH, OLED_HEIGHT, &Wire);

float receivedTemp = NAN;
bool newTemp = false;

unsigned long lastRecvMillis = 0;
bool displayCleared = false;

void onEspNowRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  if (len == sizeof(float)) {
    float temp;
    memcpy((void*)&temp, (const void*)incomingData, sizeof(float));
    receivedTemp = temp;
    newTemp = true;
    lastRecvMillis = millis();
    displayCleared = false;
  }
}

void setup() {

  Serial.begin(115200);
  delay(100);


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(onEspNowRecv);
    Serial.println("ESP-NOW initialized");
  } else {
    Serial.println("ESP-NOW initialization failed");
  }


  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(OLED_I2C_ADDR, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Waiting for temp...");
  display.display();

  lastRecvMillis = millis();
}

void loop() {

  if (newTemp) {
    float t = receivedTemp;
    newTemp = false;
    lastRecvMillis = millis();

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(30, 40);
    display.println("Vesi:");
    display.setCursor(30, 60);
    display.print(t, 1);
    display.print(" C");
    display.display();
  }


  if (!displayCleared && (millis() - lastRecvMillis >= TIMEOUT_MS)) {
    display.clearDisplay();
    display.display();
    displayCleared = true;
  }

  delay(100);
}