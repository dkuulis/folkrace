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
        info("EEPROM read %i %i\n", address, var);
    }

    if (action == EEPROM_WRITE)
    {
        message("EEPROM write %i %i\n", address, var);

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
        message("EEPROM %i %i\n", address, var);
    }
}

// perform action (r/w/reset/show) for all know addresses
void eepromAction(int action)
{
     sdCardEeprom(action);
     steerEeprom(action);
     driveEeprom(action);
     sonarEeprom(action);
     imuEeprom(action);
}

void eepromSet(const char* p)
{
    int address;
    int value;

    sscanf(p, "%i %i", &address, &value);

    message("Set EEPROM %i %i\n", address, value);
    eepromRW(address, 0, value, EEPROM_WRITE);

    eepromAction(EEPROM_READ);
    message("EEPROM reloaded\n");
}
