#include "const.h"

int processButtonEvent(const unsigned long time, int buttonEvent, int mode)
{
    if (!buttonEvent)
    {
        return mode;
    }

    if (buttonEvent == BUTTON_PRESS)
    {
        message(time, "Button pressed\n");

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
                message(time, "Unknown mode %i\n", mode);
                return MODE_IDLE;
        }
    }

    return mode;
}

int processMode(const unsigned long time, int mode)
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
                message(time, "Readiness\n");
                break;

            case MODE_COUNTDOWN:
                next = time + COUNTDOWN_INTERVAL;
                message(time, "Countdown\n");
                break;

            case MODE_RUN:
                break;

            case MODE_RUNOFF:
                next = time + RUNOFF_INTERVAL;
                message(time, "Runoff\n");
                break;

            case MODE_SONAR:
                break;

            case MODE_IMU:
                break;

            default:
                message(time, "Unknown_mode %i\n", mode);
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
                message(time, "Ready_timeout\n");
                return MODE_IDLE;

            case MODE_COUNTDOWN:
                message(time, "Starting_run\n");
                return MODE_RUN;

            case MODE_RUN:
                return mode;

            case MODE_RUNOFF:
                message(time, "Stopped\n");
                return MODE_IDLE;

            case MODE_SONAR:
                return mode;

            case MODE_IMU:
                return mode;

            default:
                message(time, "Unknown_mode %i\n", mode);
                return MODE_IDLE;
        }
    }

    return mode;
}

int processCommand(const unsigned long time, const char* command, int mode)
{
    if (!command)
    {
        return mode;
    }

    if (strcmp(command, "idle") == 0)
    {
        message(time, "Mode=idle\n");
        return MODE_IDLE;
    }
    else
    if (strcmp(command, "blink") == 0)
    {
        message(time, "Mode=blink\n");
        return MODE_BLINK;
    }
    else
    if (strcmp(command, "ready") == 0)
    {
        message(time, "Mode=ready\n");
        return MODE_READY;
    }
    else
    if (strcmp(command, "countdown") == 0)
    {
        message(time, "Mode=countdown");
        return MODE_COUNTDOWN;
    }
    else
    if (strcmp(command, "run") == 0)
    {
        message(time, "Mode=run\n");
        return MODE_RUN;
    }
    else
    if (strcmp(command, "runoff") == 0)
    {
        message(time, "Mode=runoff\n");
        return MODE_RUNOFF;
    }
    else
    if (strcmp(command, "sonar") == 0)
    {
        message(time, "Mode=sonar\n");
        return MODE_SONAR;
    }
    else
    if (strcmp(command, "imu") == 0)
    {
        message(time, "Mode=imu\n");
        return MODE_IMU;
    }
    else
    if (memcmp(command, "dump", 4) == 0    )
    {
        dumpFile(command+4, time);
        message(time, "Dump_done\n");
        return mode;
    }
    else
    if (strcmp(command, "list") == 0    )
    {
        listFiles(time);
        message(time, "Listing_done\n");
        return mode;
    }
    else
    if (strcmp(command, "ereset") == 0)
    {
        eepromAction(EEPROM_RESET, time);
        message(time, "EEPROM_reset_done");
        return mode;
    }
    else
    if (strcmp(command, "eread") == 0)
    {
        eepromAction(EEPROM_READ, time);
        message(time, "EEPROM_read_done\n");
        return mode;
    }
    else
    if (strcmp(command, "ewrite") == 0)
    {
        eepromAction(EEPROM_WRITE, time);
        message(time, "EEPROM_written\n");
        return mode;
    }
    if (strcmp(command, "eshow") == 0)
    {
        eepromAction(EEPROM_SHOW, time);
        return mode;
    }
    if (memcmp(command, "eset ", 5) == 0)
    {
        eepromSet(command+5, time); // just args
        return mode;
    }
    else
    {
        message(time, "Unknown_command %s\n", command);
        return mode;
    }
}
