#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "madya";
const char *password = "test12345";

ESP8266WebServer server(80);

const int trigPin = D1;
const int echoPin = D2;
const int irPin = D3;

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(irPin, INPUT);

    
    server.on("/", HTTP_GET, []()
    {
        server.send(200, "text/html", 
            "<h1>ESP8266 Sensor</h1>"
            "<p id='data'>Menunggu data...</p>"
            "<script>"
            "setInterval(() => {"
            "    fetch('/data')"
            "        .then(r => r.text())"
            "        .then(t => document.getElementById('data').innerHTML += '<br>' + t);"
            "}, 1000);"
            "</script>"
        ); 
    });



    server.on("/data", HTTP_GET, []()
    {
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

        String json = "{\"jarak\":" + String(distance) + ", \"ir\":" + String(irStatus) + "}";
        server.send(200, "application/json", json); 
    });

    server.begin();
}

void loop()
{
    server.handleClient();
}
