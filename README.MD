# Open Ended Question

## 1. Most Suitable Protocol for Intermittent GPS Data Transmission

**Answer:**  
**e. NB-IoT (Narrowband Internet of Things)**

---

## 2. Explanation

**NB-IoT** is specifically designed for low-power, low-bandwidth, and intermittent communication, making it ideal for devices like GPS pet collars. It offers:

- Wide coverage and good penetration (indoors and rural areas)
- Low power consumption for long battery life
- Optimized for small, infrequent data payloads

**Other options:**

- **4G/5G**: High power consumption, overkill for intermittent small data.
- **Bluetooth**: Short-range; not suitable for roaming pets.
- **WiFi**: Limited range; not practical for mobile devices.
- **LoRa**: Good for long range and low power, but less reliable for urban geolocation and less standardized for cloud integration than NB-IoT.

**Reference:**
- GSMA. [NB-IoT Explained](https://www.gsma.com/iot/narrow-band-internet-of-things-nb-iot/)
- 3GPP. [Release 13 - NB-IoT Specs](https://www.3gpp.org/release-13)

---

## 3. Securing Data Transmission

**Answer:**  
**c. HTTPS + TLS**

---

**Explanation:**  
Using **HTTPS with TLS (Transport Layer Security)** ensures:

- **Encryption** of GPS and device data in transit
- **Authentication** of the cloud server to the collar device
- **Integrity** to prevent tampering of data

**Other options:**

- **Plaintext HTTP**: No encryption, easily intercepted.
- **MQTT without TLS**: Lightweight, but insecure without TLS.
- **UDP Broadcast**: Insecure and unreliable for this use case.

**Reference:**
- OWASP. [Transport Layer Protection Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Transport_Layer_Protection_Cheat_Sheet.html)

---

## 4. Implementing Geofencing in a GPS Pet Collar

**Steps:**

1. **Define Geofence**: Use a circular or polygonal boundary defined by GPS coordinates (lat/lon).
2. **Store Coordinates**: Save geofence boundaries locally on the device or fetch from cloud.
3. **Periodic Location Checks**: GPS module checks the pet's current coordinates at intervals.
4. **Boundary Check**:
   - Use algorithms like the **Ray Casting Algorithm** for polygons or haversine formula for circular zones.
5. **Trigger Alert**: If pet exits or enters the geofence, send an alert to the cloud/app.
6. **Cloud/App Integration**: Display geofence on a map with pet’s real-time location.

**Reference:**
- Geofencing Guide by Google: [Geofencing API](https://developer.android.com/training/location/geofencing)
- Stack Overflow: [Point in Polygon Algorithm](https://stackoverflow.com/questions/217578)

---

## 5. Debugging Random Disconnection from the Cloud

**Steps:**

1. **Check Network Signal Strength**:
   - Use device diagnostics to log cellular or NB-IoT signal levels.
2. **Inspect Power Supply**:
   - Ensure the battery is stable; sudden drops can cause reboots/disconnects.
3. **Analyze Logs**:
   - Enable persistent logs (local or remote buffer) for connection states and errors.
4. **Evaluate Firmware**:
   - Check for bugs in reconnect logic, watchdog timers, or memory leaks.
5. **Test Server Stability**:
   - Ensure the cloud service is not dropping connections due to rate limits or inactivity.
6. **Simulate in Lab**:
   - Replicate disconnection patterns under controlled conditions (e.g., signal fading tests).

**Reference:**
- Nordic Semiconductor. [Debugging Cellular IoT Devices](https://www.nordicsemi.com/Products/Low-power-cellular-IoT/nRF9160)
- Microsoft Azure IoT. [Reliable Device-to-Cloud Messaging](https://learn.microsoft.com/en-us/azure/iot-hub/iot-hub-devguide-messaging)

---

# Technical Challenge

Firstly, I don’t have ESP32 and I didn’t want to buy a new one. So I looked for an online IoT platform and I found [wokwi.com](http://wokwi.com) and I used esp32 board inside this platform to get data but actually, it can not give me precise data e.g. 1st time data of battery percentage might be 50 but 2nd time might be 90. So, it is showing data randomly.
> **Note:**  
> Wokwi does **not** support real GPS data because it:
> - Simulates hardware only (does not interact with actual GPS satellites).
> - Cannot access real-world satellite GPS in simulation.
> - Only supports GPS module simulation (such as NEO-6M) via pre-defined NMEA sentences, which means the GPS data is not real but generated or scripted for simulation purposes.

For implementing a button to download data in csv format, I use Firebase to deploy a web interface and download data which is from wokwi.com platform.

So, basically, I run the C code inside [wokwi.com](http://wokwi.com) and deliver data to Firebase database and the web page collects data from Firebase.

---

## Challenge Overview

![Overview](/Technical%20Challenge/screenshots/overview.png)

For a detailed step-by-step challenge implementation, [click this link to see the challenge](/Technical%20Challenge/README.MD).
