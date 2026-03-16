#pragma once
#include <Wire.h>

#if __has_include("wifi_credentials.h")
#include "wifi_credentials.h"
#else
#define WIFI_SSID "moja_siec" // <-- tu wpisz nazwę swojej sieci WiFi
#define WIFI_PASSWORD "moje_haslo" // <-- tu wpisz hasło do swojej sieci WiFi
#endif

#define SERIAL_BAUD_RATE 115200

#define ONE_WIRE_PIN D5

#define I2C_SCL_PIN D1
#define I2C_SDA_PIN D2
#define I2C_INTERRUPT_PIN D7
#define EXPANDER_ADDRESS 0x3F

#define HTTP_PORT 80

#define MEASUREMENT_FREQUENCY               5   // measurement frequency 5s 
#define MEASUREMENT_TIMEOUT_MULTIPLIER      5   // mesurement multiplier for timeout sensor

#include "modules/temperature/temperature.h"
#include "modules/webserver/ota.h"
#include "modules/webserver/webserver.h"
#include "modules/i2cscanner/i2cscanner.h"
#include "modules/display/display.h"
#include "modules/gpioexp/gpioexp.h"
#include "modules/menu/menu.h"
#include "core/data/data.h"
#include "infrastructure/eventbus/eventbus.h"
#include "infrastructure/config/config.h"
#include "infrastructure/time/time.h"


