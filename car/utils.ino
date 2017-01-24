#include "const.h"

// fatal setup failure, goes into infinite loop
void setupFail(const char *msg)
{
    // log message, not on SD as it might not be initialised
    Serial.println(msg);
    Serial4.println(msg);

    // just blink red
    ledsOff();
    blinkRedForever();
}

// log but not to BT
void info(const unsigned long time, const char * format, ...)
{
    char buffer[256];

    ultoa(time, buffer, 10);
    char* p = buffer + strlen(buffer);
    *p++ = ' ';

    va_list args;
    va_start(args, format);
    vsprintf(p,format, args);
    va_end(args);

    sdCardPrint(buffer);
    Serial.print(buffer);
}

// log everywhere
void message(const unsigned long time, const char * format, ...)
{
    char buffer[256];

    ultoa(time, buffer, 10);
    char* p = buffer + strlen(buffer);
    *p++ = ' ';

    va_list args;
    va_start(args, format);
    vsprintf(p,format, args);
    va_end(args);

    sdCardPrint(buffer);
    Serial.print(buffer);
    Serial4.println(buffer);
}
