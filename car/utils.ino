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

void datalog(const char* msg1, const char* msg2, const char* msg3, int level)
{
    sdCardPrint(msg1);
    sdCardPrint(msg2);
    sdCardPrintln(msg3);

    Serial.print(msg1);
    Serial.print(msg2);
    Serial.println(msg3);

    if (level > LOG_INFO)
    {
        Serial4.print(msg1);
        Serial4.print(msg2);
        Serial4.println(msg3);
    }
}

void datalog(const char* msg1, const int msg2, int level)
{
    char buffer[11];
    itoa(msg2, buffer, 10);

    datalog(msg1, buffer, level);
}

void datalog(const char* msg1, const int msg2, const int msg3, int level)
{
    char buffer[11];
    char buffer2[12];
    itoa(msg2, buffer, 10);
    itoa(msg3, buffer2+1, 10); // leave place for space char
    buffer2[0] = ' ';

    datalog(msg1, buffer, buffer2, level);
}
