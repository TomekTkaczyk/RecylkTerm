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
OTA ota("esp-thermo");

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    delay(6000);
    
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(100000);
    i2cScanner.begin();
    
    Serial.println("initialization PCF...");
    gpioExpander.begin(inputPins, sizeof(inputPins), outputPins, sizeof(outputPins));
    
    dataModule.begin();
    temperature.begin();
    display.begin();
    display.displayLog(F("Connecting to WiFi..."));
    webserver.begin(WIFI_SSID, WIFI_PASSWORD);
    display.displayLog(F("WiFi connected."));
    ota.begin();
    display.displayLog(F("OTA ready."));
    display.displayLog(F("IP: ") + WiFi.localIP().toString());
    menu.begin(&mainMenu[0]);
}

void loop()
{
    timeSynchronize();
    ota.handle();
    webserver.handleClient();
    temperature.update();
    gpioExpander.update();
    eventBus.update();
}
