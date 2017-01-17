#include "const.h"

void btSerialSetup()
{
    Serial4.begin(9600); // Default connection rate for my BT module
}

// check if full command has been received
char* btSerialLoop(unsigned long time)
{
    static char buffer[SERIAL_BUFFER];
    static int end = 0;

    while (Serial4.available() > 0)
    {
        char c = Serial4.read();

        if (c == SERIAL_END1 || c == SERIAL_END2 || end == SERIAL_BUFFER - 1)
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
