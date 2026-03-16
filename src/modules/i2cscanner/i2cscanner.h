#pragma once

#include <Arduino.h>
#include <Wire.h>

class I2CScanner
{
public:
    I2CScanner(TwoWire *wire) {};

    void begin();

private:
    uint8_t count = 0;
};
