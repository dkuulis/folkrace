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

// the loop routine runs over and over again forever
void loop()
{
    // get global loop time
    unsigned long time = millis();

    // get command input
    char* command;
    command = usbSerialLoop(time);
    command = command ? command : btSerialLoop(time); // if no USB serial commnad, try bluetooth
    int buttonEvent= buttonLoop(time);

    // udpate modes and execute commands
    mode = processCommand(time, command, mode);
    mode = processButtonEvent(time, buttonEvent, mode);
    mode = processMode(time, mode);

    // component actions
    ledLoop(time, mode);
    sonarLoop(time, mode);
    imuLoop(time, mode);

    // drive
    driveLoop(time, mode);
}

// accessor
void setIdle()
{
    mode = MODE_IDLE;
}

