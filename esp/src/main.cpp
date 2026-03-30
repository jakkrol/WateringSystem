#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>

const char* ssid = "System_Nawadniania_ESP";
const char* password = NULL; 

AsyncWebServer server(80);
DNSServer dnsServer;

String soil = "0";      
String water = "0";     
String temp = "0";      
String hum = "0";       
String press = "0";     
boolean pump = false;     

void setup() {
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, 19, 18);

  if (!LittleFS.begin()) {
    Serial.println("Błąd montowania LittleFS!");
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP uruchomiony. IP: ");
  Serial.println(IP);

  dnsServer.start(53, "*", IP);

  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"soil\":" + soil + ",";
    json += "\"water\":" + water + ",";
    json += "\"temp\":" + temp + ",";
    json += "\"hum\":" + hum + ",";
    json += "\"press\":" + press + ",";
    json += "\"pump\":" + String(pump ? "true" : "false"); 
    json += "}";
    request->send(200, "application/json", json);
  });

  // Obsługa plików statycznych z pamięci Flash
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "text/javascript");
  });

  // Captive Portal - przekierowanie
  server.onNotFound([](AsyncWebServerRequest *request){
    request->redirect("/");
  });

  server.begin();
  Serial.println("Serwer WWW gotowy.");
}

void loop() {
  dnsServer.processNextRequest();

  if (Serial2.available() > 0) {
    String rawData = Serial2.readStringUntil('\n');
    rawData.trim();

    int c1 = rawData.indexOf(',');
    int c2 = rawData.indexOf(',', c1 + 1);
    int c3 = rawData.indexOf(',', c2 + 1);
    int c4 = rawData.indexOf(',', c3 + 1);
    int c5 = rawData.indexOf(',', c4 + 1);

    if (c5 != -1) {
      soil = rawData.substring(0, c1);
      water = rawData.substring(c1 + 1, c2);
      temp = rawData.substring(c2 + 1, c3);
      hum = rawData.substring(c3 + 1, c4);
      press = rawData.substring(c4 + 1, c5);
      //pump ma być boolean
      pump = (rawData.substring(c5 + 1) == "1");

      // Debug
      Serial.println("Odebrano: " + rawData);
    }
  }
}