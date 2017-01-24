#include <EEPROM.h>

#include "const.h"

void eepromSetup()
{
}

void eepromRW(int address, int defaultValue, int &var, int action, const unsigned long time)
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
        info(time, "EEPROM read %i %i\n", address, var);
    }

    if (action == EEPROM_WRITE)
    {
        message(time, "EEPROM write %i %i\n", address, var);

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
        message(time, "EEPROM %i %i\n", address, var);
    }
}

// perform action (r/w/reset/show) for all know addresses
void eepromAction(int action, const unsigned long time)
{
     sdCardEeprom(action, time);
     steerEeprom(action, time);
     driveEeprom(action, time);
     sonarEeprom(action, time);
     imuEeprom(action, time);
}

void eepromSet(const char* p, const unsigned long time)
{
    int address;
    int value;

    sscanf(p, "%i %i", &address, &value);

    message(time, "Set EEPROM %i %i\n", address, value);
    eepromRW(address, 0, value, EEPROM_WRITE, time);

    eepromAction(EEPROM_READ, time);
    message(time, "EEPROM reloaded\n");
}
