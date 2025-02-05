#include <ESP8266WiFi.h>

// Pin untuk sensor ultrasonik
const int trigPin = D1;  // GPIO5
const int echoPin = D2;  // GPIO4

// Pin untuk sensor infrared
const int irPin = D3;    // GPIO0

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irPin, INPUT);
}

void loop() {
  // Baca sensor ultrasonik
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  // Baca sensor infrared
  int irStatus = digitalRead(irPin);

  // Kirim data ke Serial Monitor via USB
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.print(" cm | Status IR: ");
  Serial.println(irStatus);

  delay(1000);
}