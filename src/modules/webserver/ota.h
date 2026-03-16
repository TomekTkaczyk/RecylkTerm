#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

class OTA
{
public:
    OTA(const char *hostname) : _hostname(hostname) {}
    void begin();
    void handle();

private:
    const char *_hostname;
};
