#include <Servo.h>
#include <i2c_t3.h>

#include "const.h"

Servo drive;

static int drivePulse = DRIVE_ZERO;

void driveSetup()
{
    drive.attach(DRIVE_PIN);
    Wire2.begin(); // enable ESC I2C
    drive.writeMicroseconds(drivePulse);
}

void driveLoop(unsigned long time, int mode)
{
    static unsigned long next = 0;

    if (next <= time) // we are past next PWM update
    {
        next  = time + DRIVE_INTERVAL;

        int old = drivePulse;
        drivePulse = getDrivePulse(mode);

        if (old != drivePulse)
        {
            datalog("Driving ", drivePulse, LOG_INFO);
            drive.writeMicroseconds(drivePulse);
        }
    }
}

int getDrivePulse(int mode)
{
    if (mode != MODE_RUN)
    {
        return DRIVE_ZERO;
    }

    int freespace = sonarDistance(0); // space ahead
    if (freespace > 0 && freespace < 50) // have reading and it's too short
    {
        datalog("Stopping at ", freespace, LOG_WARNING);
        setIdle();
        return DRIVE_ZERO;
    }

    return DRIVE_MAX;
}

