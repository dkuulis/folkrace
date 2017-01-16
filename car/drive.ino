#include <Servo.h>
#include <i2c_t3.h>

#include "const.h"

Servo drive;

void driveSetup()
{
    drive.attach(DRIVE_PIN);
    Wire2.begin(); // enable ESC I2C
}
