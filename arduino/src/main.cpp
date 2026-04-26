#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

SoftwareSerial espSerial(8, 9); // RX, TX

Adafruit_BME280 bme; 

const int MOSFET_PIN = 3; 
const int SENSOR_PIN = A0;
const int HCR_trig = 4; 
const int HCR_echo = 5; 


const int DRY_THRESHOLD = 700;  
const int WET_THRESHOLD = 600;  
const int WATER_MIN_DISTANCE = 14; 

void setup() {
  Serial.begin(115200);
  // Serial do ESP32 (SoftwareSerial najlepiej działa na 9600)
  espSerial.begin(9600);
  
  // BME280 init
  if (!bme.begin(0x76)) {
    Serial.println("Błąd: Nie znaleziono BME280 (sprawdź piny A4/A5)!");
  }

  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, LOW); 

  pinMode(HCR_trig, OUTPUT);
  pinMode(HCR_echo, INPUT);

  Serial.println("System Arduino uruchomiony.");
}

void loop() {
  int soilVal = analogRead(SENSOR_PIN);

  digitalWrite(HCR_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(HCR_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCR_trig, LOW);
  
  long duration = pulseIn(HCR_echo, HIGH, 30000);
  int distance = (duration > 0) ? (duration * 0.034 / 2) : 999;

  float t = bme.readTemperature();
  float h = bme.readHumidity();
  float p = bme.readPressure() / 100.0F;

  bool pumpState = false;
  
  if (soilVal > DRY_THRESHOLD && distance < WATER_MIN_DISTANCE && distance > 0) {
      digitalWrite(MOSFET_PIN, HIGH);
      pumpState = true;
  } 
  else if (soilVal < WET_THRESHOLD) {
      digitalWrite(MOSFET_PIN, LOW);
      pumpState = false;
  }
  else if (distance >= WATER_MIN_DISTANCE) {
      digitalWrite(MOSFET_PIN, LOW);
      pumpState = false;
  }

  espSerial.print(soilVal);   espSerial.print(",");
  espSerial.print(distance);  espSerial.print(",");
  espSerial.print(t);         espSerial.print(",");
  espSerial.print(h);         espSerial.print(",");
  espSerial.print(p);         espSerial.print(",");
  espSerial.println(pumpState ? "1" : "0"); 

  Serial.print("Soil: "); Serial.print(soilVal);
  Serial.print(" | Dist: "); Serial.print(distance);
  Serial.print(" | Temp: "); Serial.print(t);
  Serial.print(" | Pump: "); Serial.println(pumpState ? "ON" : "OFF");

  delay(2000); 
}