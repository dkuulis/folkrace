// This sketch sets servo speed based on serial input via I2C

#include "i2c_t3.h"

void setup()
{
    Wire2.begin();
    
    Serial.begin(115200);

    while (!Serial); // Leonardo: wait for serial monitor
    Serial.println(F("\nServo serial - enter desired speed in ms"));
}

int v = 0;

void loop()
{
    // if there's any serial available, read it:
    while (Serial.available() > 0)
    {
        // look for the next valid integer in the incoming serial stream:
        v = Serial.parseInt();

        Serial.print(F("I2C set "));
        Serial.println(v);
    }
  
    write_i2c(v);
    delay(50);
}

void write_i2c(byte d)
{
    Wire2.beginTransmission(0x29);
    Wire2.write(d); // write the data    
    byte error = Wire2.endTransmission(); 
    
    if (error != 0)
    {
        Serial.print(F("Unknow error "));
        Serial.println(error);
    }   
}

