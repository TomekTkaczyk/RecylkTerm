#include "temperature.h"

void Temperature::begin()
{
    dallasSensors.begin();
    uint8_t count = dallasSensors.getDeviceCount();
    for (uint8_t i = 0; i < count; i++)
    {
        DeviceAddress deviceAddress;
        if (dallasSensors.getAddress(deviceAddress, i))
        {
            char sensorId[17];
            fmtAddress(deviceAddress, sensorId);
            Serial.print(F("1Wire sensor found at index "));
            Serial.print(i);
            Serial.print(F(" id: "));
            Serial.println(sensorId);
        }
        else
        {
            Serial.print(F("Unable to find id for sensor index "));
            Serial.println(i);
        }
    }
}

void Temperature::update()
{
    switch (state)
    {
    case IDLE:
        if (timeSince(lastMeasurement) >= config.measurementInterval)
        {
            dallasSensors.setWaitForConversion(false);
            dallasSensors.requestTemperatures();
            state = WAITING_CONVERSION;
            conversionStart = millis();
        }
        break;

    case WAITING_CONVERSION:
        if (millis() - conversionStart >= conversionTime)
        {
            readTemperatures();
            state = IDLE;
            lastMeasurement = time(nullptr);
        }
        break;
    }
}

void Temperature::readTemperatures()
{
    DeviceAddress deviceAddress;

    for (uint8_t i = 0; i < dallasSensors.getDeviceCount(); i++)
    {
        if (dallasSensors.getAddress(deviceAddress, i))
        {
            Event event;
            event.type = EventType::EVT_TEMPERATURE_READY;
            fmtAddress(deviceAddress, event.data.temperature.sensorId);
            event.data.temperature.temperature = dallasSensors.getTempC(deviceAddress);
            event.data.temperature.readTimestamp = lastMeasurement;
            eventBus.publish(event);
        }
        else
        {
            Serial.print(F("Unable to find address for device index "));
            Serial.println(i);
        }
    }
}

void Temperature::fmtAddress(const DeviceAddress &addr, char *out)
{
    sprintf(out, "%02X%02X%02X%02X%02X%02X%02X%02X",
            addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]);
}
