#include "const.h"
#include "mpu9250.h"

// an MPU9250 object with its I2C address
// of 0x68 (ADDR to GRND) and on Teensy 3.5 bus 0
static MPU9250 IMU(0x68, 0, I2C_PINS_18_19, I2C_PULLUP_EXT);

static float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
static volatile float d[10];
static volatile int count = 0;

static int imuInterval = IMU_INTERVAL;

void imuSetup()
{
    int status = IMU.begin(ACCEL_RANGE_4G, GYRO_RANGE_500DPS);

    if (status != 0)
    {
        setupFail("IMU initialization unsuccessful");
    }

    imuEeprom(EEPROM_READ, 0);

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

    status = IMU.setFilter(DLPF_BANDWIDTH_184HZ, 9);
    if (status < 0) 
    {
        setupFail("IMU INT unsuccessful");
    }
    pinMode(IMU_INT_PIN, INPUT);
    attachInterrupt(IMU_INT_PIN, getIMU, RISING);    
}

void imuEeprom(int action, const unsigned long time)
{
    eepromRW(EEPROM_IMU_INTERVAL, IMU_INTERVAL, imuInterval, action, time);
}

void getIMU()
{
    // get the accel (m/s/s), gyro (rad/s), and magnetometer (uT), and temperature (C) data
    IMU.getMotion10(d);
    count++;
}

void imuLoop(unsigned long time, int mode)
{
    static unsigned long next;
    float d2[10];

    // is it this time to read IMU
    if (next < time )
    {
        // set up next ping time
        //next = time + imuInterval;
        next = time + 2;

        // get the accel (m/s/s), gyro (rad/s), and magnetometer (uT), and temperature (C) data
        //IMU.getMotion10Unbiased(d);
        

        // Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
        // the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
        // We have to make some allowance for this orientation mismatch in feeding the output to the quaternion filter.
        // For the MPU9250+MS5637 Mini breakout the +x accel/gyro is North, then -y accel/gyro is East. So if we want te quaternions properly aligned
        // we need to feed into the Madgwick function Ax, -Ay, -Az, Gx, -Gy, -Gz, My, -Mx, and Mz. But because gravity is by convention
        // positive down, we need to invert the accel data, so we pass -Ax, Ay, Az, Gx, -Gy, -Gz, My, -Mx, and Mz into the Madgwick
        // function to get North along the accel +x-axis, East along the accel -y-axis, and Down along the accel -z-axis.
        // This orientation choice can be modified to allow any convenient (non-NED) orientation convention.
        // Pass gyro rate as rad/sx
        //MadgwickQuaternionUpdate(q, d, IMU.delta);

        // print the data
        if (count > 0) 
        if (mode == MODE_IMU || mode == MODE_RUN || mode == MODE_COUNTDOWN || mode == MODE_RUNOFF)
        {
            cli();
            int c = count;
            for (int i = 0; i < 10; i++)
               d2[i] = d[i];
            count = 0;
            sei();
            printData(time, d2, c);
        }
    }
}

void printData(unsigned long time, const float* data, const int cnt)
{   /*
    float roll;
    float pitch;
    float yaw;

    toEulerianAngle(q, roll, pitch, yaw);*/

//    info(time, "IMU a %f %f %f g %f %f %f m %f %f %f q %f %f %f %f roll %f pitch %f yaw %f d %f\n",
    info(time, "IMU a %f %f %f g %f %f %f m %f %f %f c %i\n",
        data[0], data[1], data[2], // accel
        data[3], data[4], data[5], // gyro
        data[6], data[7], data[8], // magnetometer
        //q[0], q[1], q[2], q[3], // quaternion
        //roll, pitch, yaw,
        //IMU.delta
        cnt
        );
}
