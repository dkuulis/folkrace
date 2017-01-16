#include "const.h"

// set up button
void buttonSetup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// check for button press 
int buttonPressed(unsigned long time)
{
    static int state = HIGH;
    static int lastButtonState = HIGH;   // the previous reading from the input pin
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

    // read the state of the switch into a local variable:
    int reading = digitalRead(BUTTON_PIN);

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
    
    if ((time - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state
        state = reading;
    }

    // invert as pressed button is LOW, released is HIGH
    return !state;
}

