#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "common.h"
#include "./infrastructure/eventbus/eventbus.h"
#include "./infrastructure/config/config.h"

struct SensorName
{
        char sensorId[SENSOR_ID_LEN];
        uint8_t index;
        char name[SENSOR_NAME_LEN];
};

struct SensorData
{
    char sensorId[SENSOR_ID_LEN];
    time_t sensorReadTimestamp;
    float temperature;
};

struct Repository
{
    SensorData sensorsData[MAX_SENSORS];
    uint8_t sensorsDataCount = 0;
    SensorName sensorsName[MAX_SENSORS];
    uint8_t sensorsNameCount = 0;
};

class Data
{
public:
    Data(EventBus &eventBus, const SystemConfig &config) 
    : eventBus(eventBus), config(config) {}

    void begin();
    void handleTemperatureReadyEvent(const Event &e);
    SensorData* getAllSensorData(uint8_t* count); 
    
private:
    EventBus &eventBus;
    const SystemConfig &config;
    Repository repository;
    static void handleTemperatureReady(void *ctx, const Event& e);
    static void handleSensorRemoved(void *ctx, const Event& e);
};
