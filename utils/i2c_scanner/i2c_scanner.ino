// This sketch tests the standard 7-bit I2C addresses
// Devices with higher bit address might not be seen properly.
//
// see i2c discussion on  http://www.gammon.com.au/forum/?id=10896

#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  while (!Serial); // Leonardo: wait for serial monitor
  Serial.println(F("\nI2C Scanner"));
}

void loop()
{
  byte error;
  byte address;
  int nDevices;

  Serial.println(F("Scanning..."));

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
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
    Serial.println(F("No I2C devices found\n"));
  }
  else
  {
    Serial.println(F("done\n"));
  }

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
