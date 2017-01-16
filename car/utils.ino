#include "const.h"

void setupFail(const char *msg)
{
    // log message
    Serial.println(msg);
    ledsOff();
    blinkRedForever();
}
