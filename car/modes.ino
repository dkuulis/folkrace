#include "const.h"

int processButtonEvent(unsigned long time, int buttonEvent, int mode)
{
    if (!buttonEvent)
    {
        return mode;
    }

    if (buttonEvent == BUTTON_PRESS)
    {
        message("Button pressed\n");

        switch (mode)
        {
            case MODE_IDLE:
                return MODE_READY;

            case MODE_BLINK:
                return MODE_IDLE;

            case MODE_READY:
                return MODE_COUNTDOWN;

            case MODE_COUNTDOWN:
                return MODE_READY;

            case MODE_RUN:
                return MODE_IDLE;

            case MODE_RUNOFF:
                return MODE_IDLE;

            case MODE_SONAR:
                return MODE_IDLE;

            case MODE_IMU:
                return MODE_IDLE;

            default:
                message("Unknown mode %i\n", mode);
                return MODE_IDLE;
        }
    }

    return mode;
}

int processMode(unsigned long time, int mode)
{
    static int previous = -1;
    static unsigned long next = 0;

    if (previous != mode)
    {
        next = 0; // reset time

        switch (mode)
        {
            case MODE_IDLE:
                break;

            case MODE_BLINK:
                break;

            case MODE_READY:
                next = time + READY_TIMEOUT;
                message("Starting timout\n");
                break;

            case MODE_COUNTDOWN:
                next = time + COUNTDOWN_INTERVAL;
                message("Starting countdown\n");
                break;

            case MODE_RUN:
                break;

            case MODE_RUNOFF:
                next = time + RUNOFF_INTERVAL;
                message("Breaking\n");
                break;

            case MODE_SONAR:
                break;

            case MODE_IMU:
                break;

            default:
                message("Unknown mode %i\n", mode);
        }

        previous = mode;
    }

    if (next && next <= time)
    {
        next = 0; // reset time

        switch (mode)
        {
            case MODE_IDLE:
                return mode;

            case MODE_BLINK:
                return mode;

            case MODE_READY:
                message("Ready timeout\n");
                return MODE_IDLE;

            case MODE_COUNTDOWN:
                message("Starting run\n");
                return MODE_RUN;

            case MODE_RUN:
                return mode;

            case MODE_RUNOFF:
                message("Stopped\n");
                return MODE_IDLE;

            case MODE_SONAR:
                return mode;

            case MODE_IMU:
                return mode;

            default:
                message("Unknown mode %i\n", mode);
                return MODE_IDLE;
        }
    }

    return mode;
}

int processCommand(unsigned long time, char* command, int mode)
{
    if (!command)
    {
        return mode;
    }

    if (strcmp(command, "idle") == 0)
    {
        message("Mode - idle\n");
        return MODE_IDLE;
    }
    else
    if (strcmp(command, "blink") == 0)
    {
        message("Mode - blink\n");
        return MODE_BLINK;
    }
    else
    if (strcmp(command, "ready") == 0)
    {
        message("Mode - ready\n");
        return MODE_READY;
    }
    else
    if (strcmp(command, "countdown") == 0)
    {
        message("Mode - countdown");
        return MODE_COUNTDOWN;
    }
    else
    if (strcmp(command, "run") == 0)
    {
        message("Mode - run\n");
        return MODE_RUN;
    }
    else
    if (strcmp(command, "runoff") == 0)
    {
        message("Mode - runoff\n");
        return MODE_RUNOFF;
    }
    else
    if (strcmp(command, "sonar") == 0)
    {
        message("Mode - sonar\n");
        return MODE_SONAR;
    }
    else
    if (strcmp(command, "imu") == 0)
    {
        message("Mode - imu\n");
        return MODE_IMU;
    }
    else
    if (memcmp(command, "dump", 4) == 0    )
    {
        dumpFile(command+4);
        message("Dump done\n");
        return mode;
    }
    else
    if (strcmp(command, "list") == 0    )
    {
        listFiles();
        message("Listing done\n");
        return mode;
    }
    else
    if (strcmp(command, "ereset") == 0)
    {
        eepromAction(EEPROM_RESET);
        message("EEPROM reset done");
        return mode;
    }
    else
    if (strcmp(command, "eread") == 0)
    {
        eepromAction(EEPROM_READ);
        message("EEPROM read done\n");
        return mode;
    }
    else
    if (strcmp(command, "ewrite") == 0)
    {
        eepromAction(EEPROM_WRITE);
        message("EEPROM written\n");
        return mode;
    }
    if (strcmp(command, "eshow") == 0)
    {
        eepromAction(EEPROM_SHOW);
        return mode;
    }
    if (memcmp(command, "eset ", 5) == 0)
    {
        eepromSet(command+5); // just args
        return mode;
    }
    else
    {
        message("Unknown command %s\n", command);
        return MODE_IDLE;
    }
}
