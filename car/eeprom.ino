#include <EEPROM.h>

#include "const.h"

void eepromSetup()
{
    EEPROM.update(0, 1);
}
