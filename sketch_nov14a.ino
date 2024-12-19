#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

const char* ssid = "xxxxx";           // Replace with your Wi-Fi SSID
const char* password = "xxxx";   // Replace with your Wi-Fi Password

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

    // Connect to Wi-Fi
    setupWiFi();
}

void setupWiFi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
}

void sendCoordinates(float latitude, float longitude) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wi-Fi connected, sending data...");
        
        HTTPClient http;
        http.begin("http://192.168.130.138:3000/api/coordinates"); // Replace with your server endpoint
        http.addHeader("Content-Type", "application/json");

        String payload = "{\"latitude\":" + String(latitude, 6) + ",\"longitude\":" + String(longitude, 6) + "}";
        
        int httpResponseCode = http.POST(payload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("Server Response: ");
            Serial.println(response);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("Wi-Fi not connected");
    }
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
        
        if (gps.location.isUpdated()) {
            float latitude = gps.location.lat();
            float longitude = gps.location.lng();

            Serial.print("Latitude: "); Serial.println(latitude, 6);
            Serial.print("Longitude: "); Serial.println(longitude, 6);

            sendCoordinates(latitude, longitude);
            delay(10000);
        }
    }
}
