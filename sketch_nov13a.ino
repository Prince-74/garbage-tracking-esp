#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

const char* ssid = "xxxxxx";       // Replace with your Wi-Fi SSID
const char* password = "xxxxxx"; // Replace with your Wi-Fi Password

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);           // Use UART1 on ESP32 for GPS

void setup() {
    Serial.begin(115200);              // Serial monitor output
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX for GPS module
    
    setupWiFi();                       // Connect to Wi-Fi
}

void setupWiFi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
}

void sendCoordinates(float latitude, float longitude) {
    if (WiFi.status() == WL_CONNECTED) {    // Check Wi-Fi connection
        HTTPClient http;
        
        // Replace with your server URL or IP and endpoint
        http.begin("http://192.168.130.138:3000/api/coordinates");
        http.addHeader("Content-Type", "application/json");

        // Create JSON payload
        String payload = "{\"latitude\":" + String(latitude, 6) + ",\"longitude\":" + String(longitude, 6) + "}";
        
        // Send POST request
        int httpResponseCode = http.POST(payload);
        
        // Check response code
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("Server Response: ");
            Serial.println(response);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        
        http.end(); // End the HTTP request
    } else {
        Serial.println("Wi-Fi not connected");
    }
}

void loop() {
    if (gpsSerial.available() > 0) {
        Serial.println("Receiving data from GPS...");  // Debug line to confirm data from GPS
        gps.encode(gpsSerial.read());
        
        if (gps.location.isUpdated()) {
            float latitude = gps.location.lat();
            float longitude = gps.location.lng();

            Serial.print("Latitude: "); Serial.println(latitude, 6);
            Serial.print("Longitude: "); Serial.println(longitude, 6);

            sendCoordinates(latitude, longitude);
            delay(10000);
        }
    } else {
        Serial.println("No data from GPS module");  // Message if no data from GPS
        delay(10000);  // Delay to avoid spamming the serial monitor
    }
}
