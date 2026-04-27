#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>

AsyncWebServer server(80);
DNSServer dnsServer;

String soil="0", water="0", temp="0", hum="0", press="0";
String drySet="700", tankSet="20";
boolean pump = false;

void setup() {
  Serial.begin(115200);
  // Conn with Arduino
  Serial2.begin(9600, SERIAL_8N1, 19, 18);
  
  if(!LittleFS.begin()){
    Serial.println("Błąd LittleFS!");
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Smart_Garden_AP");
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String j = "{";
    j += "\"soil\":"+soil+",\"water\":"+water+",\"temp\":"+temp+",";
    j += "\"hum\":"+hum+",\"press\":"+press+",\"pump\":"+(pump?"true":"false")+",";
    j += "\"drySet\":"+drySet+",\"tankSet\":"+tankSet+"}";
    request->send(200, "application/json", j);
  });

  server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("dry")) Serial2.println("SET_DRY:"+request->getParam("dry")->value());
    if(request->hasParam("tank")) Serial2.println("SET_TANK:"+request->getParam("tank")->value());
    request->send(200, "text/plain", "OK");
  });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.onNotFound([](AsyncWebServerRequest *request){ request->redirect("/"); });
  server.begin();
  Serial.println("Serwer ESP32 gotowy!");
}

void loop() {
  dnsServer.processNextRequest();
  
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    
    Serial.print("Surowe dane: ");
    Serial.println(data);

    int c[7]; 
    c[0] = data.indexOf(',');
    for(int i=1; i<7; i++) c[i] = data.indexOf(',', c[i-1]+1);
    
    if(c[6] != -1) {
      soil = data.substring(0, c[0]);
      water = data.substring(c[0]+1, c[1]);
      temp = data.substring(c[1]+1, c[2]);
      hum = data.substring(c[2]+1, c[3]);
      press = data.substring(c[3]+1, c[4]);
      pump = (data.substring(c[4]+1, c[5]) == "1");
      drySet = data.substring(c[5]+1, c[6]);
      tankSet = data.substring(c[6]+1);
      
      Serial.println(">>> Dane zsynchronizowane pomyślnie.");
    } else {
      Serial.println("!!! Błąd formatu danych (zbyt mało przecinków)!");
    }
  }
}