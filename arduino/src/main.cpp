#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME280_ADDRESS 0x76
Adafruit_BME280 bme;

const int MOSFET_PIN = 3; 
const int SENSOR_PIN = A0;

const int HCR_trig = 4; 
const int HCR_echo = 5; 

const int DRY_THRESHOLD = 700;  
const int WET_THRESHOLD = 600;  

const int WATER_MIN_DISTANCE = 20; 

void setup() {
  Serial.begin(9600);

  // BME280 
  // if (!bme.begin(BME280_ADDRESS )) {
  //   Serial.println("Can't find BME280!");
  //   while(1) delay(10);
  // }

  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, LOW); 

  pinMode(HCR_trig, OUTPUT);
  pinMode(HCR_echo, INPUT);

  Serial.println("System nawadniania: Test czujników i pompy.");
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);

  digitalWrite(HCR_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(HCR_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCR_trig, LOW);
  
  long duration = pulseIn(HCR_echo, HIGH, 30000); // Timeout 30ms
  int distance = duration * 0.034 / 2;

  Serial.print("Wilgotność gleby: ");
  Serial.print(sensorValue);
  Serial.print(" | Dystans: ");
  Serial.print(distance);
  Serial.println(" cm");


  // --- (MOSFET) ---
  if (sensorValue > DRY_THRESHOLD) {
    // Is there water?
    if (distance > 0 && distance < WATER_MIN_DISTANCE) {
      Serial.println(">>> Sucho i jest woda. Otwieram zawór.");
      digitalWrite(MOSFET_PIN, HIGH);
    } else {
      Serial.println("!!! Sucho, ale BRAK WODY (zabezpieczenie pompy).");
      digitalWrite(MOSFET_PIN, LOW);
    }
  } 
  else if (sensorValue < WET_THRESHOLD) {
    Serial.println("--- Wilgotno. Zamykam zawór.");
    digitalWrite(MOSFET_PIN, LOW);
  }


  // Serial.print("Temperature in degC = ");
  // Serial.println(bme.readTemperature());
  // Serial.print("Pressure in hPa     = ");
  // Serial.println(bme.readPressure() / 100.0F);
  // Serial.print("Humidity in %RH     = ");
  // Serial.println(bme.readHumidity());

  delay(2000); 
}