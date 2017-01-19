#include <EEPROM.h>

#include "const.h"

void eepromSetup()
{
}

void eepromRW(int address, int defaultValue, int &var, int action)
{
    if (action == EEPROM_READ)
    {
        short v;
        EEPROM.get(address, v);
        if (v != -1) 
        {
            var = v;
        }
        else
        {
            var = defaultValue;
        }
        datalog("EEPROM read ", address, var, LOG_WARNING);
    }

    if (action == EEPROM_WRITE)
    {
        datalog("EEPROM write ", address, var, LOG_WARNING);
        
        short v = var;
        EEPROM.update(address, v & 0xFF);
        EEPROM.update(address+1, (v >> 8 ) & 0xFF);        
    }

    if (action == EEPROM_RESET)
    {
        var = defaultValue;
    }

    if (action == EEPROM_SHOW)
    {
        datalog("EEPROM ", address, var, LOG_WARNING);
    }
}

// perform action (r/w/reset/show) for all know addresses
void eepromAction(int action)
{
     sdCardEeprom(action);
     driveEeprom(action);
     steerEeprom(action);
}

void eepromSet(const char* p)
{
    int address;
    int value;
    
    sscanf(p, "%i %i", &address, &value); 

    datalog("Set EEPROM ", address, value, LOG_WARNING);
    eepromRW(address, 0, value, EEPROM_WRITE);
    
    eepromAction(EEPROM_READ);
}
