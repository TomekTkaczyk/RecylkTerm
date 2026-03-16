#pragma once

#include "common.h"
#include <Arduino.h>

// struct Event;
// typedef void (*EventCallback)(void* ctx, const Event& event);

enum class EventType : uint8_t
{
    // special null event
    EVT_NONE = 0,
    // inputs
    EVT_BUTTON_PRESS,
    EVT_BUTTON_RELEASE,
    EVT_BUTTON_CLICK,
    EVT_BUTTON_DOUBLE_CLICK,
    EVT_BUTTON_LONG_PRESS,
    EVT_BUTTON_REPEAT,
    // temperature sensor
    EVT_FOUND_SENSOR,
    EVT_SENSOR_REMOVED,
    EVT_TEMPERATURE_READY,
    // display
    EVT_DISPLAY_SENSOR_SCREEN,
    EVT_DISPLAY_MENU_SCREEN,
    EVT_DISPLAY_SETTINGS_SCREEN
    // ... other events
};

struct ButtonEventData
{
    uint8_t pin;
};

struct FoundSensorEventData
{
    char sensorId[SENSOR_ID_LEN]; 
};

struct SensorRemovedEventData
{
    char sensorId[SENSOR_ID_LEN]; // 16 chars + null terminator
};

struct TemperatureEventData
{
    char sensorId[SENSOR_ID_LEN]; // 16 chars + null terminator
    float temperature;
    time_t readTimestamp;
};

struct DisplaySensorData
{
    char name[SENSOR_NAME_LEN];
    char sensorId[SENSOR_ID_LEN]; // 16 chars + null terminator
    float temperature;
};

struct DisplaySettingData
{
    char settingName[32];
    float value;
};

#define MAX_MENU_OPTIONS 6
#define MAX_MENU_OPTION_LENGTH 6
struct DisplayMenuData
{
    char options[MAX_MENU_OPTIONS][MAX_MENU_OPTION_LENGTH]; // max 6 options, each up to 32 chars
    uint8_t optionCount;                                    // how many options are actually used
    uint8_t selectedIndex;                                  // which option is currently selected
};

struct Event
{
    EventType type;
    union
    {
        ButtonEventData button;
        FoundSensorEventData foundSensor;
        SensorRemovedEventData removedSensor;
        TemperatureEventData temperature;
        DisplaySensorData displaySensor;
        DisplayMenuData displayMenu;
        DisplaySettingData displaySetting;
    } data;
};


