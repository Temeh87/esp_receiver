#include <Arduino.h>

#define LED_PIN 2   // ESP32-dev kitin sisäänrakennettu LED on yleensä pinnissä 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // LED päälle
  delay(500);                   // odota 0,5s
  digitalWrite(LED_PIN, LOW);   // LED pois
  delay(500);                   // odota 0,5s
}
