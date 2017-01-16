#include "const.h"

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Starting up"));    
    
    // component setups
    ledSetup();
    btSerialSetup();
    sdCardSetup();

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

    // gather some inputs
    int buttonState = buttonPressed(time);

    // component actions
    ledLoop(time, buttonState);
    sonarLoop(time);
    imuLoop(time);
}


