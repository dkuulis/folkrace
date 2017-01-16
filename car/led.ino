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
void ledLoop(unsigned long time, int mode)
{
    static unsigned long next = 0;
    static bool state = false;
    static int previous = -1;

    // reset timimg on mode change
    if (previous != mode)
    {
       next = time;
       previous = mode;
       state = false; // will be blinked to true immediately
    }

    if (next <= time) // we are past next status transition
    {
        next  = time + LED_PULSE;
        state = !state; // change state to blink

        digitalWrite(LED_PIN, state && (mode == MODE_BLINK || mode == MODE_SONAR || mode == MODE_IMU));
        digitalWrite(RED_LED_PIN, state && (mode == MODE_BLINK));
        digitalWrite(YELLOW_LED_PIN, state && (mode == MODE_BLINK || mode == MODE_COUNTDOWN));
        digitalWrite(GREEN_LED_PIN, (state && (mode == MODE_BLINK || mode == MODE_READY)) || mode == MODE_IDLE);
        digitalWrite(LEFT_BLUE_LED_PIN, (state && mode == MODE_BLINK) || (!state && mode == MODE_RUN));
        digitalWrite(RIGHT_BLUE_LED_PIN, state && (mode == MODE_BLINK || mode == MODE_RUN));
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
