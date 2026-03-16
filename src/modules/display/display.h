#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./infrastructure/eventbus/eventbus.h"

#define I2C_ADDRESS 0x3C
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define OLED_RESET -1
#define SENSOR_ID_LENGTH 16

class Display
{
public:
    Display(TwoWire *wire, EventBus &eventBus)
    : display(DISPLAY_WIDTH, DISPLAY_HEIGHT, wire, OLED_RESET), eventBus(eventBus) {};

    void begin();
    void update();

    // void clear();
    // void print(const String &text);
    // void println(const String &text);

    // void setCursor(int x, int y);
    // void setTextSize(uint8_t size);
    // void setTextColor(uint16_t color);
    void displayError(const String &message);
    void displayLog(const String &message);

private:
    static void SensorScreen(void* ctx, const Event &e);
    static void MenuScreen(void* ctx, const Event &e);
    static void SettingsScreen(void* ctx, const Event &e);

    Adafruit_SSD1306 display;
    EventBus &eventBus;

    static const uint8_t MAX_LINES = 16;

    String logLines[MAX_LINES];
    uint8_t logIndex = 0;
    uint8_t logCount = 0;

};
