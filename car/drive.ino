#include <Servo.h>
#include <i2c_t3.h>

#include "const.h"

Servo driveServo;

static int driveZero = DRIVE_ZERO;
static int driveMin = DRIVE_MIN;
static int driveMax = DRIVE_MAX;
static int driveInterval = DRIVE_INTERVAL;

static int drive = DRIVE_ZERO;

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
        updateDrive(mode);

        if (old != drive)
        {
            datalog("Driving ", drive, LOG_INFO);
            driveServo.writeMicroseconds(drive);
        }

        // do steering 
        steerLoop();
    }
}

void updateDrive(int mode)
{
    if (mode != MODE_RUN)
    {
        drive = driveZero;
        return;
    }

    int freespace = sonarDistance(0); // space ahead
    if (freespace > 0 && freespace < 50) // have reading and it's too short
    {
        datalog("Stopping at ", freespace, LOG_WARNING);
        setIdle();

        drive = driveZero;
        return;
    }

    drive = driveMax;
}

void driveEeprom(int action)
{
    eepromRW(EEPROM_DRIVE_ZERO, DRIVE_ZERO, driveZero, action);
    eepromRW(EEPROM_DRIVE_MIN, DRIVE_MIN, driveMin, action);
    eepromRW(EEPROM_DRIVE_MAX, DRIVE_MAX, driveMax, action);
    eepromRW(EEPROM_DRIVE_INTERVAL, DRIVE_INTERVAL, driveInterval, action);
}
