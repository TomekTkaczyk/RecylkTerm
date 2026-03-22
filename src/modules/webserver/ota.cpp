#include "ota.h"

void OTA::begin(const char *hostname, const char *password)
{
    if (!MDNS.begin(hostname))
    {
        Serial.println("mDNS initialization error.");
        return;
    }

    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(password);

    ArduinoOTA.onStart([]()
                       { Serial.println(F("OTA Start")); });

    ArduinoOTA.onEnd([]()
                     { Serial.println(F("\nOTA End")); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total); });

    ArduinoOTA.onError([](ota_error_t error)
                       { Serial.printf("OTA Error[%u]\n", error); });

    ArduinoOTA.begin();

    Serial.println(F("OTA up and running"));
    Serial.print(F("OTA name: "));
    Serial.print(hostname);
    Serial.println(F(".local"));
}

void OTA::handle()
{
    if (WiFi.status() != WL_CONNECTED)
        return;
    ArduinoOTA.handle();
}
