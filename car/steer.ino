#include <Servo.h>

#include "const.h"

Servo steerServo;

static int steerZero = STEER_ZERO;
static int steerMin = STEER_MIN;
static int steerMax = STEER_MAX;

static int steer = STEER_ZERO;

void steerSetup()
{
    steerServo.attach(STEER_PIN);

    steerEeprom(EEPROM_READ, 0);

    steer = steerZero;
    steerServo.writeMicroseconds(steer);
}

// invoked from drive loop
void steerLoop(const unsigned long time)
{
    if (steer != steerZero) // TODO - temp code
    {
        steer = steerZero;
        message(time, "Steering %i\n", steer);
        steerServo.writeMicroseconds(steer);
    }
}

void steerEeprom(int action, const unsigned long time)
{
    eepromRW(EEPROM_STEER_ZERO, STEER_ZERO, steerZero, action, time);
    eepromRW(EEPROM_STEER_MIN, STEER_MIN, steerMin, action, time);
    eepromRW(EEPROM_STEER_MAX, STEER_MAX, steerMax, action, time);
}
