#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
unsigned int address = 0;
byte value;

void setup()
{

  Serial.begin(115200);
}

void loop()
{
    // read a byte from the current address of the EEPROM
    value = EEPROM.read(address);
  
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
  
  
    address = address + 1;
    if (address == 100)
    {
        address = 0;
    }
    
    delay(250);
}
