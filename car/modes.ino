#include "const.h"

int processButtonEvent(unsigned long time, int buttonEvent, int mode)
{
    if (!buttonEvent)
    {
        return mode;
    }

    if (buttonEvent == BUTTON_PRESS)
    {
        datalog("Button pressed", LOG_WARNING);

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

            case MODE_SONAR:
                return MODE_IDLE;

            case MODE_IMU:
                return MODE_IDLE;

            default:
                datalog("Unknown mode", LOG_ERROR);
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
                datalog("Starting timout", LOG_WARNING);
                break;

            case MODE_COUNTDOWN:
                next = time + COUNTDOWN_INTERVAL;
                datalog("Starting countdown", LOG_WARNING);
                break;

            case MODE_RUN:
                break;

            case MODE_SONAR:
                break;

            case MODE_IMU:
                break;

            default:
                datalog("Unknown mode", LOG_ERROR);
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
                datalog("Ready timeout", LOG_WARNING);
                return MODE_IDLE;

            case MODE_COUNTDOWN:
                datalog("Starting run", LOG_WARNING);
                return MODE_RUN;

            case MODE_RUN:
                return mode;

            case MODE_SONAR:
                return mode;

            case MODE_IMU:
                return mode;

            default:
                datalog("Unknown mode", LOG_ERROR);
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
        datalog("Mode - idle", LOG_WARNING);
        return MODE_IDLE;
    }
    else
    if (strcmp(command, "blink") == 0)
    {
        datalog("Mode - blink", LOG_WARNING);
        return MODE_BLINK;
    }
    else
    if (strcmp(command, "ready") == 0)
    {
        datalog("Mode - ready", LOG_WARNING);
        return MODE_READY;
    }
    else
    if (strcmp(command, "countdown") == 0)
    {
        datalog("Mode - countdown", LOG_WARNING);
        return MODE_COUNTDOWN;
    }
    else
    if (strcmp(command, "run") == 0)
    {
        datalog("Mode - run", LOG_WARNING);
        return MODE_RUN;
    }
    else
    if (strcmp(command, "sonar") == 0)
    {
        datalog("Mode - sonar", LOG_WARNING);
        return MODE_SONAR;
    }
    else
    if (strcmp(command, "imu") == 0)
    {
        datalog("Mode - imu", LOG_WARNING);
        return MODE_IMU;
    }
    else
    if (memcmp(command, "dump", 4) == 0    )
    {
        dumpFile(command+4);
        datalog("Dump done", LOG_WARNING);
        return mode;
    }
    else
    if (strcmp(command, "ereset") == 0)
    {
        datalog("Resetting EEPROM", LOG_WARNING);
        eepromAction(EEPROM_RESET);
        datalog("Reset done", LOG_WARNING);
        return mode;
    }
    else
    if (strcmp(command, "eread") == 0)
    {
        datalog("Reading EEPROM", LOG_WARNING);
        eepromAction(EEPROM_READ);
        datalog("Read done", LOG_WARNING);
        return mode;
    }
    else
    if (strcmp(command, "ewrite") == 0)
    {
        datalog("Writing EEPROM", LOG_WARNING);
        eepromAction(EEPROM_WRITE);
        datalog("Write done", LOG_WARNING);
        return mode;
    }
    if (strcmp(command, "eshow") == 0)
    {
        datalog("Show EEPROM", LOG_WARNING);
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
        datalog("Unknown command ", command, LOG_ERROR);
        return MODE_IDLE;
    }
}
