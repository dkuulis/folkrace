#include <Servo.h>
#include <i2c_t3.h>

#include "const.h"

Servo driveServo;

static int driveZero = DRIVE_ZERO;
static int driveMin = DRIVE_MIN;
static int driveMax = DRIVE_MAX;
static int driveInterval = DRIVE_INTERVAL;
static int speedInterval = SPEED_INTERVAL;
static int driveSlowForward = DRIVE_SLOW_FORWARD;
static int driveSlowBackward = DRIVE_SLOW_BACKWARD;
static int driveRange = DRIVE_RANGE;

static int drive = DRIVE_ZERO;

#define SUBMODE_IDLE 0
#define SUBMODE_RUN 1
#define SUBMODE_SLOW 2

static int submode = SUBMODE_IDLE;

void driveSetup()
{
    driveServo.attach(DRIVE_PIN);
    Wire2.begin(I2C_MASTER, 0,  I2C_PINS_3_4, I2C_PULLUP_EXT, 100000);  // enable ESC I2C

    driveEeprom(EEPROM_READ);

    drive = driveZero;
    driveServo.writeMicroseconds(drive);
}

void driveLoop(unsigned long time, int mode)
{
    static unsigned long next = 0;

    if (next <= time) // we are past next PWM update
    {
        next  = time + driveInterval;

        int old = drive;
        updateDrive(time, mode);

        if (old != drive)
        {
            message("Driving %i\n", drive);
            driveServo.writeMicroseconds(drive);
        }

        // do steering
        steerLoop();
    }
}

void updateDrive(unsigned long time, int mode)
{
    // not runnig
    if (mode != MODE_RUN)
    {
        drive = driveZero;
        submode = SUBMODE_IDLE;
        return;
    }

    // set run submode if main mode is run and submode is IDLE
    if (submode == SUBMODE_IDLE)
    {
        submode = SUBMODE_RUN;
    }

    driveSubmodeLoop(time, mode);
    speedLoop(time, mode);
}

void driveSubmodeLoop(unsigned long time, int mode)
{
    static unsigned long next = 0;

    int freespace = sonarDistance(0); // space ahead
    if (freespace != 0 && next <= time) // we are past next PWM update - and do we have valid reading - TODO - differentiate ping zeros...
    {
        next  = nextSonarUpdate(0);

        if (/*freespace > 0 && */ freespace < driveRange) // have reading and it's too short
        {
            switch (submode)
            {
                case SUBMODE_RUN:
                    message("Slowing dist %i @ %lu\n", freespace, time);
                    submode = SUBMODE_SLOW;
                    break;

                case SUBMODE_SLOW:
                    message("Stopping dist %i @ %lu\n", freespace, time);
                    submode = SUBMODE_IDLE;
                    break;

                case SUBMODE_IDLE:
                    message("?? idle submode ??\n");
                    break;
            }
        }
        else
        {
            if (submode != SUBMODE_RUN)
            {
                message("Resuming run %i @ %lu\n", freespace, time);
                submode = SUBMODE_RUN; // we have space...
            }
        }
    }
}

void speedLoop(unsigned long time, int mode)
{
    /*
    static unsigned long next = 0;
    static int speed = DRIVE_ZERO;
    if (next <= time) // we are past next PWM update
    {
        next = time + speedInterval;

        if (submode == SUBMODE_RUN && speed > driveMin)
        {
            speed -= 1;
        }
    }*/

    switch (submode)
    {
        case SUBMODE_RUN:
            drive = driveMax;
            // drive = speed;
            break;

        case SUBMODE_SLOW:
            drive = driveSlowForward;
            // drive = speed;
            break;

        case SUBMODE_IDLE:
            //speed = driveZero;
            drive = driveZero;
            endRun(); // set also main mode
    }
}

void driveEeprom(int action)
{
    eepromRW(EEPROM_DRIVE_ZERO, DRIVE_ZERO, driveZero, action);
    eepromRW(EEPROM_DRIVE_MIN, DRIVE_MIN, driveMin, action);
    eepromRW(EEPROM_DRIVE_MAX, DRIVE_MAX, driveMax, action);
    eepromRW(EEPROM_DRIVE_INTERVAL, DRIVE_INTERVAL, driveInterval, action);
    eepromRW(EEPROM_SPEED_INTERVAL, SPEED_INTERVAL, speedInterval, action);
    eepromRW(EEPROM_DRIVE_SLOW_FORWARD, DRIVE_SLOW_FORWARD, driveSlowForward, action);
    eepromRW(EEPROM_DRIVE_SLOW_BACKWARD, DRIVE_SLOW_BACKWARD, driveSlowBackward, action);
    eepromRW(EEPROM_DRIVE_RANGE, DRIVE_RANGE, driveRange, action);}
