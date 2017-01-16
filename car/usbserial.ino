#include "const.h"

void usbSerialSetup()
{
    Serial.begin(115200);
}

char* usbSerialLoop(unsigned long time)
{
    static char buffer[SERIAL_BUFFER];
    static int end = 0;

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == SERIAL_END1 || c == SERIAL_END2 || end == SERIAL_BUFFER - 1) // TODO - pick right character terminator
        {
            if (end == 0)
            {
                break;
            }

            buffer[end] = 0;
            end = 0;

            return buffer;
        }

        buffer[end++] = c;
    }

    return 0;
}
