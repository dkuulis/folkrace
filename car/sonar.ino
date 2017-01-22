#include <NewPing.h>

#include "const.h"

static int pingInterval = PING_INTERVAL;

// Sensor object array
NewPing sonar[SONAR_NUM] =
{
    NewPing(SONAR1_TRIG_PIN, SONAR1_ECHO_PIN, MAX_DISTANCE),
/*    NewPing(SONAR2_TRIG_PIN, SONAR2_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR3_TRIG_PIN, SONAR3_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR4_TRIG_PIN, SONAR4_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR5_TRIG_PIN, SONAR5_ECHO_PIN, MAX_DISTANCE)*/
};

void sonarSetup()
{
    sonarEeprom(EEPROM_READ);
}

void sonarEeprom(int action)
{
    eepromRW(EEPROM_PING_INTERVAL, PING_INTERVAL, pingInterval, action);
}

static int currentSensor = 0;
static volatile int distance[SONAR_NUM]; // volatile as changed from ISR

static unsigned long nextSonar;

void sonarLoop(unsigned long time, int mode)
{
    // is it this time to ping?
    if (nextSonar < time)
    {
        // make sure previous timer is canceled before starting a new ping (insurance).
        sonar[currentSensor].timer_stop();

        if (mode == MODE_SONAR || mode == MODE_RUN || mode == MODE_COUNTDOWN || mode == MODE_RUNOFF)
        {
            // report prev result
            info("Sonar %i = %i cm @ %lu\n", currentSensor, distance[currentSensor], time);
        }

        // set up next ping time
        nextSonar = time + pingInterval;

        // choose next sensor, clear results
        currentSensor = (currentSensor + 1) % SONAR_NUM;
        distance[currentSensor] = 0;

        // do the ping (processing continues, interrupt will call echoCheck to look for echo).
        sonar[currentSensor].ping_timer(echoCheck);
    }
}

void echoCheck()
{
    // if ping received, set the sensor distance
    if (sonar[currentSensor].check_timer())
    {
        distance[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    }
}

// accessor
int sonarDistance(int sensor)
{
    return distance[sensor];
}

unsigned long nextSonarUpdate(int sensor)
{
    int count = (sensor + SONAR_NUM - currentSensor) % SONAR_NUM;
    return nextSonar + count*pingInterval;
}

