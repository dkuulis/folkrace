#include "const.h"

// initialize LEDs
void ledSetup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(LEFT_BLUE_LED_PIN, OUTPUT);
    pinMode(RIGHT_BLUE_LED_PIN, OUTPUT);
}

// blink LEDs in loop, omit some if button is pressed
void ledLoop(unsigned long time, int buttonState)
{
    static unsigned long next = 0;
    static bool state = false;

    if (next < time) // we are past next status transition
    {
        next  = time + LED_PULSE;
        state = !state; // change state to blink

        // no blinking on non-blue LEDs if button is pressed
        int otherState = buttonState ? LOW : state;

        digitalWrite(LED_PIN, otherState);
        digitalWrite(RED_LED_PIN, otherState);
        digitalWrite(YELLOW_LED_PIN, otherState);
        digitalWrite(GREEN_LED_PIN, otherState);

        // blues always blink
        digitalWrite(LEFT_BLUE_LED_PIN, state);
        digitalWrite(RIGHT_BLUE_LED_PIN, state);
    }
}

void ledsOff()
{
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(LEFT_BLUE_LED_PIN, LOW);
    digitalWrite(RIGHT_BLUE_LED_PIN, LOW);
}

void blinkRedForever()
{
    while (true)
    {
        digitalWrite(RED_LED_PIN, HIGH);
        delay(LED_PULSE);
        digitalWrite(RED_LED_PIN, LOW);
        delay(LED_PULSE);
    }
}
