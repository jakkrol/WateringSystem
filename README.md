# 🌿 Smart Irrigation System (IoT)

A dual-processor automated plant watering and environmental monitoring system. This project features a real-time web dashboard served via an **ESP32 Captive Portal**, with a dedicated **Arduino Uno** handling sensor acquisition and high-power switching.

---

## 🚀 Key Features
* **Dual-Processor Architecture:** Offloads timing-critical sensor tasks to Arduino while the ESP32 manages high-level networking and the Web Server.
* **Automated Irrigation:** Intelligent moisture-based control via a high-power MOSFET valve driver.
* **Environmental Monitoring:** High-precision air temperature, humidity, and pressure sensing using the **BME280** sensor.
* **Safety Interlocks:** Ultrasonic water level sensing (**HC-SR04**) to prevent pump dry-run and monitor reservoir levels.
* **Dynamic Configuration:** Real-time adjustment of watering thresholds and reservoir capacity directly from the Web UI.
* **Non-Volatile Memory (NVM):** User settings (thresholds) are stored in the Arduino's **EEPROM**, ensuring they persist even after a power failure.
* **Responsive Dashboard:** Modern UI built with CSS Flexbox/Grid and JavaScript (Async/Await Fetch API).
* **Captive Portal:** Automatic redirection to the dashboard upon Wi-Fi connection for easy "headless" operation.

---

## 🛠 Hardware Architecture
The system utilizes a **bidirectional** serial communication bridge (UART) through a **Logic Level Shifter** to ensure signal integrity between the 5V (Arduino) and 3.3V (ESP32) domains.

### 1. Arduino Uno (Data Acquisition & Control)
* **Sensors:** Soil Moisture (Analog), HC-SR04 (Ultrasonic), BME280 (I2C).
* **Execution:** MOSFET control for the 12V water valve.
* **Persistence:** Manages the **EEPROM** storage for `DRY_THRESHOLD` and `TANK_CAPACITY`.
* **Communication:** Transmits CSV data and receives configuration commands via `SoftwareSerial`.

### 2. ESP32 (Network Gateway & Web Server)
* **Networking:** Host for the `AsyncWebServer` and local DNS server.
* **Data Handling:** Parses incoming UART data into JSON and forwards UI commands back to the Arduino.
* **Storage:** Serves frontend assets (HTML/CSS/JS) from the integrated **LittleFS** flash memory.

---

## ⚙️ Persistent Settings & Sync
The system features a closed-loop synchronization mechanism:
1. **At Startup:** Arduino reads saved values from **EEPROM** and sends them to the ESP32 via Serial.
2. **UI Initialization:** The Web Dashboard receives these values via JSON and automatically adjusts the sliders to match the device's actual state.
3. **Real-time Update:** When a user moves a slider, the new value is sent through the ESP32 to the Arduino, which immediately updates its logic and overwrites the EEPROM.

---

## 💻 Tech Stack
* **Firmware:** C++ (Arduino Framework / PlatformIO)
* **Backend:** ESPAsyncWebServer, LittleFS, DNSServer, EEPROM.h
* **Frontend:** HTML5, CSS3, JavaScript (ES6+ Fetch API)
* **Data Format:** JSON (API), CSV (Internal Serial Bridge)

---

## 📦 Installation & Deployment
1. **Prepare Assets:** Upload the contents of the `/data` folder to the ESP32 using the **PlatformIO: Upload Filesystem Image** task.
2. **Flash Arduino:** Upload the control sketch to the Arduino Uno.
3. **Flash ESP32:** Upload the server sketch to the ESP32.
4. **Connect:** 
   * Search for Wi-Fi SSID: `Smart_Garden_AP`.
   * The Captive Portal should trigger automatically. If not, navigate to `192.168.4.1` in your browser.
