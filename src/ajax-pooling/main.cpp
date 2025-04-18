#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

const char *ssid = "madya";
const char *password = "test12345";

ESP8266WebServer server(80);
Ticker ultrasonicTicker;
Ticker infraredTicker;

const int echoPin = D7;
const int trigPin = D8;

volatile float distance = 0;
volatile int irStatus = 0;

void connectWiFi()
{
    Serial.print("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());
}

volatile float lastValidDistance = -1;

void readUltrasonic()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseInLong(echoPin, HIGH, 30000);

    float newDistance = duration * 0.034 / 2;

    if (newDistance > 2 && newDistance < 400)
    {
        distance = newDistance;
        lastValidDistance = distance;
    }
    else if (lastValidDistance != -1)
    {
        distance = lastValidDistance;
    }
}

void handleRoot()
{
    String html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP8266 Sensor Data</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 20px; text-align: center; }
                .data-container { margin: 10px auto; padding: 10px; border: 1px solid #ddd; width: 300px; border-radius: 10px; text-align: left; }
                .data-label { font-weight: bold; }
                .data-value { color: green; font-size: 20px; }
            </style>
            <script>
                function fetchData() {
                    fetch('/ultrasonic')
                        .then(response => response.json())
                        .then(data => {
                            document.getElementById('ultrasonic-data').innerText = data.distance_cm + ' cm';
                            if (data.distance_cm < 20) {
                                document.getElementById('status-objek').innerText = 'Ada Kain';
                            } else {
                                document.getElementById('status-objek').innerText = 'Tidak Ada Kain';
                            }
                        });
                }
                setInterval(fetchData, 100);
            </script>
        </head>
        <body>
            <h1>ESP8266 Sensor Data</h1>
            <div class="data-container">
                <span class="data-label">Ultrasonic Distance:</span>
                <span class="data-value" id="ultrasonic-data">-</span>
            </div>
            <div class="data-container">
                <span class="data-label">Status:</span>
                <span class="data-value" id="status-objek">-</span>
            </div>
        </body>
        </html>
    )";
    server.send(200, "text/html", html);
}

void handleUltrasonic()
{
    String json = "{\"distance_cm\": " + String(distance) + "}";
    server.send(200, "application/json", json);
}

void setup()
{
    Serial.begin(115200);
    connectWiFi();

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    server.on("/", HTTP_GET, handleRoot);
    server.on("/ultrasonic", HTTP_GET, handleUltrasonic);

    server.begin();

    ultrasonicTicker.attach_ms(50, readUltrasonic);
}

void loop()
{
    server.handleClient();
}
