#include "const.h"

void setupFail(const char *msg)
{
    // log message
    Serial.println(msg);
    ledsOff();
    blinkRedForever();
}

void datalog(const char* msg, int level)
{
    sdCardPrintln(msg);
    Serial.println(msg);

    if (level > LOG_INFO)
    {
        Serial4.println(msg);
    }
}


void datalog(const char* msg1, const char* msg2, int level)
{
    sdCardPrint(msg1);
    sdCardPrintln(msg2);

    Serial.print(msg1);
    Serial.println(msg2);

    if (level > LOG_INFO)
    {
        Serial4.print(msg1);
        Serial4.println(msg2);
    }
}

void datalog(const char* msg1, const int msg2, int level)
{
    char buffer[10]; //the ASCII of the integer will be stored in this char array
    itoa(msg2, buffer, 10); //(integer, yourBuffer, base)

    datalog(msg1, buffer, level);
}
