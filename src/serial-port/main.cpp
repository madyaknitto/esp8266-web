#include <Ticker.h>

const int echoPin = D7;
const int trigPin = D8;

volatile unsigned long startTime = 0;
volatile unsigned long endTime = 0;
volatile bool measuring = false;
volatile bool newDistanceAvailable = false;

float distance = 0.0;
unsigned long lastTriggerTime = 0;
const unsigned long TRIGGER_INTERVAL_MS = 60;

void IRAM_ATTR echoISR() {
    if (digitalRead(echoPin) == HIGH) {
        startTime = micros();
        measuring = true;
    } else if (measuring) {
        endTime = micros();
        measuring = false;
        newDistanceAvailable = true;
    }
}

void triggerUltrasonic() {
    unsigned long currentTime = millis();
    if (currentTime - lastTriggerTime >= TRIGGER_INTERVAL_MS) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        lastTriggerTime = currentTime;
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(echoPin), echoISR, CHANGE);
}

void loop() {
    if (newDistanceAvailable) {
        noInterrupts();
        unsigned long duration = endTime - startTime;
        newDistanceAvailable = false;
        interrupts();

        distance = duration * 0.034 / 2.0;

        if (distance > 0 && distance < 400.0) {
            Serial.print("{\"ultrasonic\":");
            Serial.print(distance);
            Serial.println("}");
        } else {
            Serial.print("{\"ultrasonic\":");
            Serial.print(125.0);
            Serial.println("}");
        }
    }

    triggerUltrasonic();
}
