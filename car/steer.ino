#include <Servo.h>

#include "const.h"

Servo steer;
static int steerPulse = STEER_ZERO;

void steerSetup()
{
    steer.attach(STEER_PIN);
    steer.writeMicroseconds(steerPulse);
}
