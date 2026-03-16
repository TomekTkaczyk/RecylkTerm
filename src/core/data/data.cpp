#include "data.h"

void Data::begin()
{
    eventBus.subscribe(EventType::EVT_TEMPERATURE_READY, Data::handleTemperatureReady, this);
}

void Data::handleTemperatureReady(void *ctx, const Event &e)
{
    Data *self = static_cast<Data *>(ctx);
    if (self)
    {
        self->handleTemperatureReadyEvent(e);
    }
}

void Data::handleSensorRemoved(void *ctx, const Event &e)
{
}

void Data::handleTemperatureReadyEvent(const Event &e)
{
    for (uint8_t i = 0; i < repository.sensorsDataCount; i++)
    {
        if (strcmp(repository.sensorsData[i].sensorId, e.data.temperature.sensorId) == 0)
        {
            repository.sensorsData[i].temperature = e.data.temperature.temperature;
            repository.sensorsData[i].sensorReadTimestamp = e.data.temperature.readTimestamp;
            return;
        }
    }

    if (repository.sensorsDataCount >= MAX_SENSORS)
    {
        Serial.println(F("Sensor list full"));
        return;
    }

    SensorData &sensorData = repository.sensorsData[repository.sensorsDataCount];
    strncpy(sensorData.sensorId, e.data.temperature.sensorId, sizeof(sensorData.sensorId) - 1);
    sensorData.sensorId[sizeof(sensorData.sensorId) - 1] = '\0';
    sensorData.temperature = e.data.temperature.temperature;
    sensorData.sensorReadTimestamp = e.data.temperature.readTimestamp;

    repository.sensorsData[repository.sensorsDataCount++] = sensorData;
}

SensorData *Data::getAllSensorData(uint8_t *count)
{
    time_t timeout = config.measurementInterval * config.measurementIntervalMultiplier;
    time_t now = time(nullptr);
    uint8_t newCount = 0;

    for (uint8_t i = 0; i < repository.sensorsDataCount; i++)
    {
        if (now - repository.sensorsData[i].sensorReadTimestamp <= timeout)
        {
            repository.sensorsData[newCount++] = repository.sensorsData[i];
        }
        else
        {
            // emit remove sensor event
            Event event;
            event.type = EventType::EVT_SENSOR_REMOVED;
            strncpy(event.data.removedSensor.sensorId, repository.sensorsData[i].sensorId, sizeof(event.data.removedSensor.sensorId));
            event.data.removedSensor.sensorId[sizeof(event.data.removedSensor.sensorId) - 1] = '\0';
            eventBus.publish(event);
        }
    }

    repository.sensorsDataCount = newCount;
    *count = repository.sensorsDataCount;
    return repository.sensorsData;
}

// DataModule *DataModule::instance = nullptr;

// DataModule::DataModule(EventBus &eventBus) : eventBus(eventBus)
// {
//     instance = this;
// }

// void DataModule::handleFoundSensorEvent(const Event &e)
// {
//     if (!instance)
//         return;

//     Serial.println(F("DataModule received EVT_FOUND_SENSOR"));

//     auto &repo = instance->repository;
//     const char *newId = e.data.foundSensor.sensorId;

//     // sprawdzenie czy sensor już istnieje
//     for (uint8_t i = 0; i < repo.sensorsCount; i++)
//     {
//         if (strcmp(repo.sensors[i].sensorId, newId) == 0)
//         {
//             Serial.println(F("Sensor already exists"));
//             return;
//         }
//     }

//     // sprawdzenie miejsca
//     if (repo.sensorsCount >= MAX_SENSORS)
//     {
//         Serial.println(F("Sensor list full"));
//         return;
//     }

//     // dodanie nowego sensora
//     SensorName &s = repo.sensors[repo.sensorsCount];

//     strncpy(s.sensorId, newId, sizeof(s.sensorId) - 1);
//     s.sensorId[sizeof(s.sensorId) - 1] = '\0'; // gwarancja zakończenia stringa

//     s.index = repo.sensorsCount;
//     strncpy(s.type, "DS18B20", sizeof(s.type) - 1); // jeśli masz typ sensora
//     s.type[sizeof(s.type) - 1] = '\0';              // gwarancja zakończenia stringa

//     repo.sensorsCount++;

//     Serial.print(F("New sensor added: "));
//     Serial.println(s.sensorId);

//     // zapis do repozytorium (LittleFS / JSON)
//     instance->save();
// }

// void DataModule::handleTemperatureReadyEvent(const Event &e)
// {
//     Serial.println(F("DataModule received EVT_TEMPERATURE_READY event"));

//     if (!instance)
//         return;
//     for (uint8_t i = 0; i < instance->repository.sensorsDataCount; i++)
//     {
//         if (strcmp(instance->repository.sensorsData[i].sensorId, e.data.temperature.sensorId) == 0)
//         {
//             instance->repository.sensorsData[i].temperature = e.data.temperature.temperature;
//             return;
//         }
//     }
// }

// void DataModule::begin()
// {
//     eventBus.subscribe(EventType::EVT_FOUND_SENSOR, handleFoundSensorEvent);
//     eventBus.subscribe(EventType::EVT_TEMPERATURE_READY, handleTemperatureReadyEvent);
//     // if (!LittleFS.begin())
//     //     return false;
//     // return load();
// }

// bool DataModule::load()
// {
//     if (!LittleFS.exists("/data.json"))
//         return false;

//     File conf = LittleFS.open("/data.json", "r");
//     if (!conf)
//         return false;

//     JsonDocument doc;
//     DeserializationError err = deserializeJson(doc, conf);
//     conf.close();
//     if (err)
//         return false;

//     strncpy(repository.name, doc["name"] | "Sterownik", sizeof(repository.name));
//     repository.version = doc["version"] | 1;

//     JsonArray arr = doc["sensors"].as<JsonArray>();
//     repository.sensorsCount = 0;

//     for (JsonObject s : arr)
//     {
//         if (repository.sensorsCount >= MAX_SENSORS)
//             break;
//         SensorName &sc = repository.sensors[repository.sensorsCount];
//         sc.index = s["index"] | 0;
//         strncpy(sc.sensorId, s["sensorid"] | "", sizeof(sc.sensorId));
//         strncpy(sc.type, s["type"] | "DS18B20", sizeof(sc.type));
//         strncpy(sc.name, s["name"] | "Sensor", sizeof(sc.name));
//         repository.sensorsCount++;
//     }
//     return true;
// }

// bool DataModule::save()
// {
//     JsonDocument doc;

//     doc["name"] = repository.name;
//     doc["version"] = repository.version;

//     JsonArray arr = doc["sensors"].to<JsonArray>();

//     for (uint8_t i = 0; i < repository.sensorsCount; i++)
//     {
//         JsonObject s = arr.add<JsonObject>();

//         s["index"] = repository.sensors[i].index;
//         s["sensorid"] = repository.sensors[i].sensorId;
//         s["type"] = repository.sensors[i].type;
//         s["name"] = repository.sensors[i].name;
//     }

//     File f = LittleFS.open("/data.json", "w");
//     if (!f)
//         return false;

//     serializeJsonPretty(doc, f);
//     f.close();
//     return true;
// }

// SensorData *DataModule::getAllSensorData(uint8_t &count)
// {
//     count = repository.sensorsDataCount;
//     return repository.sensorsData;
// }

// void DataModule::showData()
// {
//     File file = LittleFS.open("/data.json", "r");
//     if (!file)
//         return;

//     while (file.available())
//     {
//         String line = file.readStringUntil('\n');
//         Serial.println(line);
//     }
//     file.close();
// }

// bool DataModule::addSensor(SensorName sensor)
// {
//     if (repository.sensorCount >= MAX_SENSORS)
//         return false;
//     bool exists = false;
//     for (uint8_t i = 0; i < repository.sensorCount; i++)
//     {
//         if (strcmp(repository.sensors[i].sensorId, sensor.sensorId) == 0)
//         {
//             exists = true;
//             break;
//         }
//     }
//     if (exists)
//         return false;
//     sensor.index = repository.sensorCount;
//     strcpy(sensor.type, "DS18B20");
//     sensor.name[0] = '\0';
//     repository.sensors[repository.sensorCount++] = sensor;
//     return true;
// }

// void DataModule::updateSensorData(const SensorData sensorData)
// {
//     for (uint8_t i = 0; i < repository.sensorsDataCount; i++)
//     {
//         if (strcmp(repository.sensorsData[i].sensorId, sensorData.sensorId) == 0)
//         {
//             repository.sensorsData[i].temperature = sensorData.temperature;
//             publishTemperatureEvent(sensorData);
//             return;
//         }
//     }
// }

// void DataModule::publishTemperatureEvent(const SensorData &sensorData)
// {
//     Event event;
//     event.type = EventType::EVT_TEMPERATURE_READY;
//     strncpy(event.data.temperature.sensorId, sensorData.sensorId, sizeof(event.data.temperature.sensorId));
//     event.data.temperature.temperature = sensorData.temperature;
//     eventBus.publish(event);
// }
