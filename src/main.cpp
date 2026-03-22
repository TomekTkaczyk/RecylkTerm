#include "main.h"

EventBus eventBus;
const SystemConfig config = {MEASUREMENT_FREQUENCY, MEASUREMENT_TIMEOUT_MULTIPLIER};
I2CScanner i2cScanner(&Wire);
Data dataModule(eventBus, config);
Temperature temperature(ONE_WIRE_PIN, eventBus, config);
GPIOExpander gpioExpander(EXPANDER_ADDRESS, I2C_INTERRUPT_PIN, eventBus);
Display display(&Wire, eventBus);
Menu menu(eventBus);
WebServer webserver(dataModule);
OTA ota;

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    delay(6000);
    
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(100000);
    i2cScanner.begin();
    gpioExpander.begin(inputPins, sizeof(inputPins), outputPins, sizeof(outputPins));
    display.begin();
    dataModule.begin();
    temperature.begin();
    display.displayLog(F("Connecting to WiFi..."));
    webserver.begin(WIFI_SSID, WIFI_PASSWORD);
    if(WiFi.status() == WL_CONNECTED)
    {
        display.displayLog(F("WiFi connected."));
        display.displayLog(F("IP: ") + WiFi.localIP().toString());
        ota.begin(OTA_NAME, OTA_PASSWORD);
    }
    else 
    {
        display.displayLog(F("WiFi unavailable."));
    }
    menu.begin(&mainMenu[0]);
}

void loop()
{
    timeSynchronize();
    webserver.handleClient();
    ota.handle();
    temperature.update();
    gpioExpander.update();
    menu.update();
    eventBus.update();
}
