#include "const.h"

static int mode = MODE_IDLE;

void setup()
{
    // component setups
    ledSetup();
    usbSerialSetup();
    btSerialSetup();
    sdCardSetup();
    eepromSetup();

    buttonSetup();
    steerSetup();
    driveSetup();
    sonarSetup();
    imuSetup();
}

// the loop routine runs over and over again forever:
void loop()
{
    // get global loop time
    unsigned long time = millis();

    // handle serial command
    char* command;
    command = usbSerialLoop(time);
    command = command ? command : btSerialLoop(time); // if no USB serial commnad, try bluetooth
    processCommand(time, command);

    // gather some inputs
    int button = buttonEvent(time);
    processButtonEvent(time, button);

    processMode(time);

    // component actions
    ledLoop(time, mode);
    sonarLoop(time, mode);
    imuLoop(time, mode);

    // drive
    driveLoop(time, mode);
}

void processButtonEvent(unsigned long time, int button)
{
    if (!button)
    {
        return;
    }

    if (button == BUTTON_PRESS)
    {
        datalog("Button pressed", LOG_WARNING);

        switch (mode)
        {
            case MODE_IDLE:
                mode = MODE_READY;
                break;

            case MODE_BLINK:
                mode = MODE_IDLE;
                break;

            case MODE_READY:
                mode = MODE_COUNTDOWN;
                break;

            case MODE_COUNTDOWN:
                mode = MODE_READY;
                break;

            case MODE_RUN:
                mode = MODE_IDLE;
                break;

            case MODE_SONAR:
                mode = MODE_IDLE;
                break;

            case MODE_IMU:
                mode = MODE_IDLE;
                break;

            default:
                datalog("Unknown mode", LOG_ERROR);
        }
    }
}

void processMode(unsigned long time)
{
    static int previous = -1;
    static unsigned long next = 0;

    if (previous != mode)
    {
        next = 0;

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
        next = 0;

        switch (mode)
        {
            case MODE_IDLE:
                break;

            case MODE_BLINK:
                break;

            case MODE_READY:
                mode = MODE_IDLE;
                datalog("Ready timeout", LOG_WARNING);
                break;

            case MODE_COUNTDOWN:
                mode = MODE_RUN;
                datalog("Starting run", LOG_WARNING);
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
    }
}

void processCommand(unsigned long time, char* command)
{
    if (!command)
    {
        return;
    }

    if (strcmp(command, "idle") == 0)
    {
        mode = MODE_IDLE;
        datalog("Mode - idle", LOG_WARNING);
    }
    else
    if (strcmp(command, "blink") == 0)
    {
        mode = MODE_BLINK;
        datalog("Mode - blink", LOG_WARNING);
    }
    else
    if (strcmp(command, "ready") == 0)
    {
        mode = MODE_READY;
        datalog("Mode - ready", LOG_WARNING);
    }
    else
    if (strcmp(command, "countdown") == 0)
    {
        mode = MODE_COUNTDOWN;
        datalog("Mode - countdown", LOG_WARNING);
    }
    else
    if (strcmp(command, "run") == 0)
    {
        mode = MODE_RUN;
        datalog("Mode - run", LOG_WARNING);
    }
    else
    if (strcmp(command, "sonar") == 0)
    {
        mode = MODE_SONAR;
        datalog("Mode - sonar", LOG_WARNING);
    }
    else
    if (strcmp(command, "imu") == 0)
    {
        mode = MODE_IMU;
        datalog("Mode - imu", LOG_WARNING);
    }
    else
    if (strcmp(command, "dump") == 0)
    {
        datalog("Dumping datalog", LOG_WARNING);
        dumpFile();
        datalog("Dump done", LOG_WARNING);    }
    else
    {
        datalog("Unknown command ", command, LOG_ERROR);
    }
}

void setIdle()
{
    mode = MODE_IDLE;
}

