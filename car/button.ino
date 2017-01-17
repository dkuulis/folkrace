#include "const.h"

// set up button
void buttonSetup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// check for button press/release
int buttonLoop(unsigned long time)
{
    static int state = HIGH;
    static int lastButtonState = HIGH;   // the previous reading from the input pin
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

    // read the state of the switch into a local variable:
    int reading = digitalRead(BUTTON_PIN);
    int event = BUTTON_NONE;

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = time;

        // save the reading (no use to update variable if they are equal, so nothing in else clause)
        lastButtonState = reading;
    }

    if ((time - lastDebounceTime) > DEBOUNCE_DELAY && state != reading)
    {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state
        event = reading ? BUTTON_RELEASE : BUTTON_PRESS;
        state = reading;
    }

    return event;
}

