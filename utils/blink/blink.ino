// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0

#define LED 13
#define PULSE 200

void setup()
{
    // initialize the digital pin as an output.
    pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
    digitalWrite(LED, HIGH);
    delay(PULSE);
    digitalWrite(LED, LOW);
    delay(PULSE);
}
