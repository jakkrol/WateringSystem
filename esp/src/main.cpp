#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>

const char* ssid = "ESP";
const char* password = NULL; 

AsyncWebServer server(80);
DNSServer dnsServer;

// DANE TESTOWE 
String soil = "600";     
String water = "15";     
String temp = "24.5";    
String hum = "45.0";     
String press = "1013.2"; 
boolean pump = false;     

void setup() {
  Serial.begin(115200);

  // Getting website from flash
  if (!LittleFS.begin()) {
    Serial.println("Błąd montowania LittleFS!");
  }

  // Access point init
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point uruchomiony! Nazwa: ");
  Serial.println(ssid);
  Serial.print("Adres IP: ");
  Serial.println(IP);

  // Route dns requests to website
  dnsServer.start(53, "*", IP);

  // Frontend endpoint
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(LittleFS, "/style.css", "text/css");
});

server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(LittleFS, "/script.js", "text/javascript");
});

  server.onNotFound([](AsyncWebServerRequest *request){
    request->redirect("/");
  });

  // Getting rest of the files
  // server.serveStatic("/", LittleFS, "/");

  server.begin();
  Serial.println("Serwer WWW uruchomiony.");
}

void loop() {
  // For captive portal, dns needs to loop
  dnsServer.processNextRequest();
}