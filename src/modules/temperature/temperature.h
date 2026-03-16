#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#include "./infrastructure/time/time.h"
#include "./infrastructure/eventbus/eventbus.h"
#include "./infrastructure/config/config.h"

class Temperature
{
public:
    Temperature(uint8_t pin, EventBus &eventBus, const SystemConfig &config)
    : oneWire(pin), dallasSensors(&oneWire), eventBus(eventBus), config(config) {}
    void begin();
    void update();
    
private:
    OneWire oneWire;
    DallasTemperature dallasSensors;
    EventBus &eventBus;
    const SystemConfig &config;

    enum State { IDLE, WAITING_CONVERSION };
    State state = IDLE;

    time_t lastMeasurement = 0;
    
    unsigned long conversionStart = 0;
    static constexpr unsigned long conversionTime = 750;  // 12-bit DS18B20 750ms

    void readTemperatures();
    void fmtAddress(const DeviceAddress &addr, char *out);
};