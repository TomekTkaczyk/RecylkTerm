#include "display.h"

void Display::SensorScreen(void *ctx, const Event &e)
{
    Display *self = static_cast<Display *>(ctx);
    if (!self) return;
    
    self->display.clearDisplay();

    String name = e.data.displaySensor.name;
    String sensorId = e.data.displaySensor.sensorId;
    float temperature = e.data.displaySensor.temperature;

    self->display.setCursor(0, 0);
    self->display.setTextSize(1);
    self->display.setTextColor(SSD1306_WHITE);
    self->display.println(name + " (" + sensorId + ")");
    self->display.println("Temp: " + String(temperature, 1) + " C");
}

void Display::MenuScreen(void *ctx, const Event &e)
{
    Display *self = static_cast<Display *>(ctx);
    if (!self) return;

    self->display.clearDisplay();

    String options[MAX_MENU_OPTIONS];
    for (uint8_t i = 0; i < e.data.displayMenu.optionCount; i++)
    {
        options[i] = e.data.displayMenu.options[i];
    }
    uint8_t selectedIndex = e.data.displayMenu.selectedIndex;

    self->display.setCursor(0, 0);
    self->display.setTextSize(1);
    self->display.setTextColor(SSD1306_WHITE);
    for (uint8_t i = 0; i < e.data.displayMenu.optionCount; i++)
    {
        if (i == selectedIndex)
        {
            self->display.println("> " + options[i]);
        }
        else
        {
            self->display.println("  " + options[i]);
        }
    }
}

void Display::SettingsScreen(void *ctx, const Event &e)
{
    Display *self = static_cast<Display *>(ctx);
    if (!self) return;
    
    self->display.clearDisplay();

    String settingName = e.data.displaySetting.settingName;
    float value = e.data.displaySetting.value;

    self->display.setCursor(0, 0);
    self->display.setTextSize(1);
    self->display.setTextColor(SSD1306_WHITE);
    self->display.println(settingName);
    self->display.println("Value: " + String(value, 1));
}

void Display::begin()
{
    eventBus.subscribe(EventType::EVT_DISPLAY_SENSOR_SCREEN, Display::SensorScreen, this);
    eventBus.subscribe(EventType::EVT_DISPLAY_MENU_SCREEN, Display::MenuScreen, this);
    eventBus.subscribe(EventType::EVT_DISPLAY_SETTINGS_SCREEN, Display::SettingsScreen, this);

    if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    display.clearDisplay();
    display.display();
}

void Display::update()
{
    display.display();
}

void Display::displayError(const String &message)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.println(F("Error:"));
    display.setCursor(0, 24);
    display.println(message);
    display.display();
}

void Display::displayLog(const String &message)
{
    logLines[logIndex] = message;
    logIndex = (logIndex + 1) % MAX_LINES;
    if (logCount < MAX_LINES)
    {
        logCount++;
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    uint8_t maxDisplayLines = DISPLAY_HEIGHT / 8;
    uint8_t start = (logCount > maxDisplayLines) ? (logIndex + MAX_LINES - maxDisplayLines) % MAX_LINES : 0;
    for (uint8_t i = 0; i < min(logCount, maxDisplayLines); i++)
    {
        uint8_t idx = (start + i) % MAX_LINES;
        display.println(logLines[idx]);
    }
    display.display();
}
