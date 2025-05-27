# ESP32 GPS Data Logger (Wokwi + Firebase)

This project demonstrates how to simulate an ESP32 device that logs GPS and battery data, uploads it to Firebase Realtime Database, and provides a web interface to download the data as CSV. The hardware simulation is done using [Wokwi](https://wokwi.com/), so you don't need a physical ESP32 board.

### Here is the overview scretch of the project
![Overview](/Technical%20Challenge/screenshots/overview.png)
---

## Demo


### Wokwi Program Running

![Wokwi Program Running](/Technical%20Challenge/screenshots/wokwi.png)

### Firebase Database Setup

![Firebase Database](/Technical%20Challenge/screenshots/firebase_database.png)

### Web UI Page

![Web UI](/Technical%20Challenge/screenshots/webUI.png)

---

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Setup Instructions](#setup-instructions)
  - [1. Wokwi (ESP32 Simulation)](#1-wokwi-esp32-simulation)
  - [2. Firebase Realtime Database](#2-firebase-realtime-database)
  - [3. Web Interface](#3-web-interface)
- [Sample Data Flow](#sample-data-flow)
- [Credits](#credits)

---

## Overview

- **Device Simulation**: ESP32 board runs C code inside Wokwi to generate random GPS and battery data.
- **Data Upload**: Data is sent to Firebase Realtime Database using HTTP requests.
- **Web Interface**: A Firebase-hosted webpage fetches the data and allows CSV download.

---

## How It Works

1. The ESP32 (simulated in Wokwi) generates random values for latitude, longitude, battery percentage, and timestamp.
2. These values are sent via HTTP PUT requests to your Firebase Realtime Database.
3. The web application retrieves data from Firebase and provides a button to download the data as a CSV file.

---

## Setup Instructions

### 1. Wokwi (ESP32 Simulation)

- Go to [https://wokwi.com/](https://wokwi.com/)
- Create an account and start a new project.
- Choose **ESP32** as the board.
- Add the following code to your `sketch.ino`:

    ```c
    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <ArduinoJson.h>

    const char* ssid = "Wokwi-GUEST";
    const char* password = "";

    const char* firebaseHost = "https://esp32-gps-tracking-2bb71-default-rtdb.firebaseio.com/";
    const char* firebaseSecret = "MYrumhmcQ83uLyORjL3jtOwO9i340e6dSR7LE4lH"; // Legacy key

    const unsigned long uploadInterval = 60000; // 1 hour in milliseconds
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
      uploadCount++;
    }

    void loop() {
      unsigned long currentTime = millis();
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
    ```

- Add a `diagram.json` to define the board wiring:

    ```json
    {
      "version": 1,
      "author": "GPS Data Logger",
      "editor": "wokwi",
      "parts": [
        {
          "type": "wokwi-esp32-devkit-v1",
          "id": "esp",
          "top": 0,
          "left": 0,
          "attrs": {}
        }
      ],
      "connections": [
        [ "esp:TX0", "$serialMonitor:RX", "", [] ],
        [ "esp:RX0", "$serialMonitor:TX", "", [] ]
      ]
    }
    ```

- Click **Run** to start the simulation. You should see logs in the serial monitor and data appearing in your Firebase database.

---

### 2. Firebase Realtime Database

1. Go to [Firebase Console](https://console.firebase.google.com/).
2. **Create a new project** (or use an existing one).
3. Add a **Realtime Database** to your project (set rules accordingly for testing).
4. Copy the Database URL and generate a **database secret** (legacy token) if not already done.
5. Update the `firebaseHost` and `firebaseSecret` in your `sketch.ino` with your actual values.

---

### 3. Web Interface

- Deploy a web front-end on Firebase Hosting.
- The web page should fetch data from your Firebase Realtime Database and allow users to download the data as a `.csv` file.

---

## Sample Data Flow

1. ESP32 (simulated) generates data and uploads to Firebase.
2. Data appears under `gpsData/` node in your Firebase Realtime Database.
3. The web interface reads this data and allows CSV export.

---

## Credits

- **Wokwi**: For ESP32 simulation ([wokwi.com](https://wokwi.com))
- **Firebase**: For backend and hosting
- **Author**: [Phyo Thura]

[BACK](/README.MD).