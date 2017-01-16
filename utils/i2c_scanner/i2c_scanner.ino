// This sketch tests the standard 7-bit I2C addresses
// Devices with higher bit address might not be seen properly.
//
// see i2c discussion on  http://www.gammon.com.au/forum/?id=10896

#include "i2c_t3.h"

#define BUSES 3

void setup()
{
    Serial.begin(115200);

    while (!Serial); // Leonardo: wait for serial monitor
    Serial.println(F("I2C Scanner"));
    
    for (int bus = 0; bus<BUSES; bus++)
    {
        Serial.print(F("Initializing bus "));
        Serial.println(bus);
        i2c_t3(bus).begin();
    }
}

void loop()
{
    byte error;
    byte address;
    int nDevices;

    for (int bus = 0; bus<BUSES; bus++)
    {
        Serial.print(F("Scanning bus "));
        Serial.println(bus);
        nDevices = 0;

        for (address = 1; address < 127; address++ )
        {
            // The i2c_scanner uses the return value of
            // the Write.endTransmisstion to see if
            // a device did acknowledge to the address.

            i2c_t3(bus).beginTransmission(address);
            error = i2c_t3(bus).endTransmission();
            
            if (error == 0)
            {
                Serial.print(F("I2C device found on bus "));
                Serial.print(bus);
                Serial.print(F(" at address 0x"));
                printhex(address);

                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print(F("Unknow error at address 0x"));
                printhex(address);
            }
        }

        if (nDevices == 0)
        {
            Serial.print(F("No I2C devices found on bus "));
            Serial.println(bus);
        }
        else
        {
            Serial.println(F("done"));
        }
    }

    Serial.println();
    delay(5000); // wait 5 seconds for next scan
}

void printhex(byte b)
{
  if (b < 16)
  {
    Serial.print(F("0"));
  }

  Serial.println(b, HEX);
}
