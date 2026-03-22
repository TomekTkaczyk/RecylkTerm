#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

class OTA
{
public:
    // OTA(const char *hostname, const char* password ) : _hostname(hostname), _password(password) {}
    void begin(const char *hostname, const char* password );
    void handle();

private:
    const char *_hostname;
    const char *_password;
};
