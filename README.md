# 🌿 Smart Irrigation System (IoT)

A dual-processor automated plant watering and environmental monitoring system. This project features a real-time web dashboard served via an **ESP32 Captive Portal**, with a dedicated **Arduino Uno** handling sensor acquisition and high-power switching.

---

## 🚀 Key Features
* **Dual-Processor Architecture:** Offloads timing-critical sensor tasks to Arduino while the ESP32 manages high-level networking and the Web Server.
* **Automated Irrigation:** Intelligent moisture-based control via a high-power MOSFET valve driver.
* **Environmental Monitoring:** High-precision air temperature, humidity, and pressure sensing using the **BME280** sensor.
* **Safety Interlocks:** Ultrasonic water level sensing (**HC-SR04**) to prevent pump dry-run and monitor reservoir levels.
* **Responsive Dashboard:** Modern UI built with CSS Flexbox/Grid and JavaScript (Async/Await Fetch API).
* **Captive Portal:** Automatic redirection to the dashboard upon Wi-Fi connection for easy "headless" operation.

---

## 🛠 Hardware Architecture
The system utilizes a serial communication bridge (UART) through a **Logic Level Shifter** to ensure signal integrity between the 5V (Arduino) and 3.3V (ESP32) domains.

### 1. Arduino Uno (Data Acquisition & Control)
* **Sensors:** Soil Moisture (Analog), HC-SR04 (Ultrasonic), BME280 (I2C).
* **Execution:** MOSFET PWM control for the 12V water valve.
* **Communication:** Transmits CSV data via `SoftwareSerial` (Pin 9) to the ESP32.

### 2. ESP32 (Network Gateway & Web Server)
* **Networking:** Host for the `AsyncWebServer` and local DNS server.
* **Data Handling:** Receives UART data on `Serial2` (Pin 19) and parses it into JSON format.
* **Storage:** Serves frontend assets (HTML/CSS/JS) from the integrated `LittleFS` flash memory.

---

## 🔌 Pin Mapping (Schematic Overview)
| Component | Pin (Arduino Uno) | Pin (ESP32) | Protocol |
| :--- | :--- | :--- | :--- |
| **Soil Moisture Sensor** | A0 | - | Analog |
| **BME280 (SDA/SCL)** | A4 / A5 | - | I2C (5V VIN) |
| **HC-SR04 (Trig/Echo)** | D4 / D5 | - | Digital |
| **MOSFET (Valve Trigger)** | D3 | - | Digital/PWM |
| **Communication Bridge** | **D9 (TX)** | **D19 (RX2)** | UART (9600 bps) |

---

## 💻 Tech Stack
* **Firmware:** C++ (Arduino Framework)
* **Backend:** ESPAsyncWebServer, LittleFS, DNSServer
* **Frontend:** HTML5, CSS3, JavaScript (ES6+ Fetch API)
* **Data Format:** JSON (API), CSV (Internal Serial Bridge)

---

## 📦 Installation & Deployment
1.  **Prepare Assets:** Upload the contents of the `/data` folder to the ESP32 using the **ESP32 LittleFS Data Upload** tool.
2.  **Flash Arduino:** Upload the control sketch to the Arduino Uno.
3.  **Flash ESP32:** Upload the server sketch to the ESP32.
4.  **Connect:** * Search for Wi-Fi SSID: `System_Nawadniania_ESP`.
    * The Captive Portal should trigger automatically. If not, navigate to `192.168.4.1` in your browser.

