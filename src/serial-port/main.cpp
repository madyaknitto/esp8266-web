#include <Ticker.h>

Ticker ultrasonicTicker;
Ticker infraredTicker;

const int trigPin = D1;
const int echoPin = D2;
const int irPin = D3;

float distance = 0.0;
int irStatus = 0;

void startUltrasonicMeasurement() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
}

void readInfrared() {
    irStatus = digitalRead(irPin);
}

void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(irPin, INPUT);

    // Menjalankan fungsi secara periodik tanpa perlu mengeceknya di loop()
    ultrasonicTicker.attach_ms(100, startUltrasonicMeasurement);
    infraredTicker.attach_ms(50, readInfrared);
}

void loop() {
    static float lastDistance = -1;
    static int lastIrStatus = -1;

    if (distance != lastDistance || irStatus != lastIrStatus) {
        Serial.print("{\"ultrasonic\":");
        Serial.print(distance);
        Serial.print(",\"infrared\":");
        Serial.print(irStatus);
        Serial.println("}"); 
        lastDistance = distance;
        lastIrStatus = irStatus;
    }
}
