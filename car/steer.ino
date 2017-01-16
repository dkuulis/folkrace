#include <Servo.h>

#include "const.h"

Servo steer;

void steerSetup()
{
    steer.attach(STEER_PIN);
}
