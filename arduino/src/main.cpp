#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define PIN_SOIL A0
#define PIN_TRIG 4
#define PIN_ECHO 5
#define PIN_PUMP 3  
#define PIN_RX 8    
#define PIN_TX 9    

SoftwareSerial espSerial(PIN_RX, PIN_TX);
Adafruit_BME280 bme;

int DRY_THRESHOLD;    
int TANK_CAPACITY;    
const int ADDR_DRY = 0;
const int ADDR_TANK = 2;

unsigned long lastSend = 0;
bool pumpState = false;

void setup() {
  Serial.begin(115200); 
  espSerial.begin(9600);
  
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_PUMP, LOW);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 error!");
  }

  DRY_THRESHOLD = EEPROM.read(ADDR_DRY) * 4; 
  TANK_CAPACITY = EEPROM.read(ADDR_TANK);    

  if (DRY_THRESHOLD < 100 || DRY_THRESHOLD > 1023) DRY_THRESHOLD = 700;
  if (TANK_CAPACITY < 5 || TANK_CAPACITY > 200) TANK_CAPACITY = 20;

  Serial.println("--- SYSTEM START ---");
  Serial.print("Prog EEPROM: "); Serial.println(DRY_THRESHOLD);
  Serial.print("Zbiornik EEPROM: "); Serial.println(TANK_CAPACITY);
}

void loop() {
  if (espSerial.available() > 0) {
    String cmd = espSerial.readStringUntil('\n');
    cmd.trim();
    if (cmd.startsWith("SET_DRY:")) {
      DRY_THRESHOLD = cmd.substring(8).toInt();
      EEPROM.write(ADDR_DRY, DRY_THRESHOLD / 4);
      Serial.print("Odebrano nowy prog: "); Serial.println(DRY_THRESHOLD);
    } else if (cmd.startsWith("SET_TANK:")) {
      TANK_CAPACITY = cmd.substring(9).toInt();
      EEPROM.write(ADDR_TANK, TANK_CAPACITY);
      Serial.print("Odebrano nowa glebokosc: "); Serial.println(TANK_CAPACITY);
    }
  }

  if (millis() - lastSend > 2000) {
    int soilRaw = analogRead(PIN_SOIL);

    digitalWrite(PIN_TRIG, LOW); 
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH); 
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    long duration = pulseIn(PIN_ECHO, HIGH, 30000); 
    int distance = duration * 0.034 / 2;
    int waterLevel = TANK_CAPACITY - distance;
    if (waterLevel < 0 || duration == 0) waterLevel = 0;

    Serial.println("--- POMIAR ---");
    Serial.print("Soil Raw: "); Serial.println(soilRaw);
    Serial.print("Echo duration: "); Serial.print(duration);
    Serial.print(" | Distance: "); Serial.print(distance);
    Serial.print(" | Calculated Water: "); Serial.println(waterLevel);
    
    float t = bme.readTemperature();
    float h = bme.readHumidity();
    float p = bme.readPressure() / 100.0F;

    pumpState = (soilRaw > DRY_THRESHOLD && waterLevel > 2);
    digitalWrite(PIN_PUMP, pumpState ? HIGH : LOW);
    Serial.print("Status Pompy: "); Serial.println(pumpState ? "ON" : "OFF");

    espSerial.print(soilRaw); espSerial.print(",");
    espSerial.print(waterLevel); espSerial.print(",");
    espSerial.print(t); espSerial.print(",");
    espSerial.print(h); espSerial.print(",");
    espSerial.print(p); espSerial.print(",");
    espSerial.print(pumpState ? "1" : "0"); espSerial.print(",");
    espSerial.print(DRY_THRESHOLD); espSerial.print(",");
    espSerial.println(TANK_CAPACITY);

    lastSend = millis();
  }
}