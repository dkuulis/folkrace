// This sketch sets servo speed based on serial input

#include <Servo.h>

#define SERVO_PIN 9 // change as needed

Servo srv;

void setup()
{
  Serial.begin(115200);
  srv.attach(SERVO_PIN);

  while (!Serial); // Leonardo: wait for serial monitor
  Serial.println(F("\nServo serial - enter desired speed in ms"));
}

void loop()
{
  // if there's any serial available, read it:
  while (Serial.available() > 0)
  {

    // look for the next valid integer in the incoming serial stream:
    int pulse = Serial.parseInt();
    srv.writeMicroseconds(pulse);

    Serial.print(F("Pulse set "));
    Serial.println(pulse);
  }
}
