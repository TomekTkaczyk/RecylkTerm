#include "ota.h"

void OTA::begin()
{
    ArduinoOTA.setHostname(_hostname);

    ArduinoOTA.onStart([]() {
        Serial.println(F("OTA Start"));
    });

    ArduinoOTA.onEnd([]() {
        Serial.println(F("\nOTA End"));
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]\n", error);
    });

    ArduinoOTA.begin();

    Serial.println(F("OTA Ready"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void OTA::handle()
{
    ArduinoOTA.handle();
}
