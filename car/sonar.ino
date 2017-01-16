#include <NewPing.h>

#include "const.h"

// Sensor object array
NewPing sonar[SONAR_NUM] =
{
    NewPing(SONAR1_TRIG_PIN, SONAR1_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR2_TRIG_PIN, SONAR2_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR3_TRIG_PIN, SONAR3_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR4_TRIG_PIN, SONAR4_ECHO_PIN, MAX_DISTANCE),
    NewPing(SONAR5_TRIG_PIN, SONAR5_ECHO_PIN, MAX_DISTANCE)
};

void sonarSetup()
{
}

static int currentSensor = 0;
static volatile int distance[SONAR_NUM]; // volatile as changed from ISR

void sonarLoop(unsigned long time, int mode)
{
    static unsigned long next;

    // is it this time to ping?
    if (next < time)
    {
        // make sure previous timer is canceled before starting a new ping (insurance).
        sonar[currentSensor].timer_stop();

        if (mode == MODE_SONAR)
        {
            // report prev result
            Serial.print(F("Sonar "));
            Serial.print(currentSensor);
            Serial.print(F(" = "));
            Serial.print(distance[currentSensor]);
            Serial.println(F(" cm"));
        }

        // set up next ping time
        next = time + PING_INTERVAL;

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

int sonarDistance(int sensor)
{
    return distance[sensor];
}

