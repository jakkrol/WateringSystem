#include <Arduino.h>

const int MOSFET_PIN = 3; 
const int SENSOR_PIN = A0;

const int HCR_trig = 9;
const int HCR_echo = 10;

const int DRY_THRESHOLD = 700;  
const int WET_THRESHOLD = 600;  


void setup() {
   Serial.begin(9600);
  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, LOW); 

  pinMode(HCR_trig, OUTPUT);
  pinMode(HCR_echo, INPUT);

  Serial.println("System nawadniania uruchomiony.");
}

void loop() {
  // int sensorValue = analogRead(SENSOR_PIN);
  
  // Serial.print("Odczyt czujnika: ");
  // Serial.println(sensorValue);

  // if (sensorValue > DRY_THRESHOLD) {
  //   Serial.println(">>> Sucho! Otwieram zawór.");
  //   digitalWrite(MOSFET_PIN, HIGH);
  // } 
  // else if (sensorValue < WET_THRESHOLD) {
  //   Serial.println("--- Wilgotno. Zamykam zawór.");
  //   digitalWrite(MOSFET_PIN, LOW);
  // }

  // delay(2000); 



  ////////////////////HCR sensor logic////////////////////////////
  digitalWrite(HCR_trig, LOW);
  delayMicroseconds(2);
  
  // Wysłanie impulsu 
  digitalWrite(HCR_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCR_trig, LOW);
  
  // Odczyt 
  long duration = pulseIn(HCR_echo, HIGH);
  
  // Przeliczenie na cm 
  int distance = duration * 0.034 / 2;

  Serial.print("Dystans: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}