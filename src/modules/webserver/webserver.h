#pragma once
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include "./core/data/data.h"

class WebServer {
public:
    WebServer(Data &data, uint16_t port = 80)
    : data(data), server(port) {}

    void begin(const char* ssid, const char* password);
    void handleClient();
    
private:
    Data &data;

    ESP8266WebServer server;
    
    void handleRoot();              
    void handleTemperatures();  
    String getTemperaturesJson();
};
