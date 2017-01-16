#include "const.h"
#include "mpu9250.h"

// an MPU9250 object with its I2C address
// of 0x68 (ADDR to GRND) and on Teensy 3.5 bus 0
MPU9250 IMU(0x68, 0, I2C_PINS_18_19, I2C_PULLUP_EXT);

float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float d[10];

void imuSetup()
{
    int status = IMU.begin(ACCEL_RANGE_4G, GYRO_RANGE_500DPS);
    
    if (!status) 
    {
        setupFail("IMU initialization unsuccessful");
    }    

    float b[18] = {
            -0.66359285f-0.13f, 0.596398637f, 0.149558031f,  // accel bias
            -0.00336684f, -0.038908617f, -0.003352749f, // gyro bias

            -7.909176574f, 10.11728131f, 22.60300639f, // megnetometer bias

            // magnetometer scale matrix
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };

    IMU.setBias(b);
}

void imuLoop(unsigned long time)
{
    static unsigned long next;

    // is it this time to ping?
    if (next < time) 
    {
        // set up next ping time
        next = time + IMU_INTERVAL;

        // get the accel (m/s/s), gyro (rad/s), and magnetometer (uT), and temperature (C) data
        IMU.getMotion10Unbiased(d);

        // Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
        // the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
        // We have to make some allowance for this orientation mismatch in feeding the output to the quaternion filter.
        // For the MPU9250+MS5637 Mini breakout the +x accel/gyro is North, then -y accel/gyro is East. So if we want te quaternions properly aligned
        // we need to feed into the Madgwick function Ax, -Ay, -Az, Gx, -Gy, -Gz, My, -Mx, and Mz. But because gravity is by convention
        // positive down, we need to invert the accel data, so we pass -Ax, Ay, Az, Gx, -Gy, -Gz, My, -Mx, and Mz into the Madgwick
        // function to get North along the accel +x-axis, East along the accel -y-axis, and Down along the accel -z-axis.
        // This orientation choice can be modified to allow any convenient (non-NED) orientation convention.
        // Pass gyro rate as rad/sx

        MadgwickQuaternionUpdate(q, d, IMU.delta);

        // print the data
        printData(time);
    }
}

void printData(unsigned long time)
{
    Serial.print(F("IMU "));
    Serial.print(time);

    Serial.print(F(" ")); Serial.print(d[0], 6);
    Serial.print(F(" ")); Serial.print(d[1], 6);
    Serial.print(F(" ")); Serial.print(d[2], 6);

    Serial.print(F(" ")); Serial.print(d[3], 6);
    Serial.print(F(" ")); Serial.print(d[4], 6);
    Serial.print(F(" ")); Serial.print(d[5], 6);

    Serial.print(F(" ")); Serial.print(d[6], 6);
    Serial.print(F(" ")); Serial.print(d[7], 6);
    Serial.print(F(" ")); Serial.print(d[8], 6);

    Serial.print(F(" ")); Serial.print(q[0], 6);
    Serial.print(F(" ")); Serial.print(q[1], 6);
    Serial.print(F(" ")); Serial.print(q[2], 6);
    Serial.print(F(" ")); Serial.print(q[3], 6);

    float roll;
    float pitch;
    float yaw;

    toEulerianAngle(q, roll, pitch, yaw);

    Serial.print(F(" ")); Serial.print(roll, 6);
    Serial.print(F(" ")); Serial.print(pitch, 6);
    Serial.print(F(" ")); Serial.print(yaw, 6);

    /*
    Serial.print(F(" ")); Serial.print(d[TEMPERATURE],6); // t
    */

    Serial.println();
}
