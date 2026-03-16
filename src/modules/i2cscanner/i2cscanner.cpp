#include "i2cscanner.h"

void I2CScanner::begin()
{
    Serial.println(F("Scanning I2C bus..."));
    for (uint8_t address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0)
        {
            Serial.print(F("I2C device found at address 0x"));
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
            count++;
        }
    }
    if (count == 0)
        Serial.println(F("No I2C devices found"));
    else
        Serial.println(F("Found ") + String(count) + F(" I2C device(s)."));
}
