#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* firebaseHost = "https://esp32-gps-tracking-2bb71-default-rtdb.firebaseio.com/";
const char* firebaseSecret = "MYrumhmcQ83uLyORjL3jtOwO9i340e6dSR7LE4lH"; // Legacy key

const unsigned long uploadInterval = 360000; // 1 hour in milliseconds
unsigned long lastUploadTime;
int uploadCount = 0;
const int maxUploads = 12;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // Send first data immediately
    sendDataToFirebase();
    lastUploadTime = millis(); // Start timing after first upload
    //Serial.println(lastUploadTime);
    //Serial.println("last upload time");
    uploadCount++;
    }

    void loop() {
    unsigned long currentTime = millis();
    //Serial.println("current time");
    //Serial.println(currentTime);
    if (uploadCount < maxUploads && (currentTime - lastUploadTime) >= uploadInterval) {
        sendDataToFirebase();
        lastUploadTime = currentTime;
        uploadCount++;
}

  delay(100); // Reduce CPU usage
}

void sendDataToFirebase() {
    float latitude = 16.8409 + random(-100, 100) / 10000.0;
    float longitude = 96.1735 + random(-100, 100) / 10000.0;
    int battery = random(50, 100);

    // Timestamp simulation
    String timestamp = "2025-05-27T" + String(random(10, 24)) + ":00:00";

    HTTPClient http;
    String url = String(firebaseHost) + "/gpsData/" + String(uploadCount) + ".json?auth=" + firebaseSecret;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["latitude"] = latitude;
    doc["longitude"] = longitude;
    doc["battery"] = battery;
    doc["timestamp"] = timestamp;

    String json;
    serializeJson(doc, json);

    int httpCode = http.PUT(json);
    if (httpCode > 0) {
        Serial.println("Uploaded: " + json);
    } else {
        Serial.println("Upload failed: " + String(httpCode));
    }

    http.end();
}
