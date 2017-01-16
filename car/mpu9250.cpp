/*
Brian R Taylor
brian.taylor@bolderflight.com
2016-10-10

Copyright (c) 2016 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Arduino.h>
#include <i2c_t3.h>

#include "mpu9250.h"
#include "const.h"

/* MPU9250 object, input the I2C address, I2C bus, I2C pins, and I2C pullups */
MPU9250::MPU9250(uint8_t address, uint8_t bus, i2c_pins pins, i2c_pullup pullups)
{
    _address = address; // I2C address
    _bus = bus; // I2C bus
    _pins = pins; // I2C pins
    _pullups = pullups; // I2C pullups
}

/* starts I2C communication and sets up the MPU-9250 */
int MPU9250::begin(mpu9250_accel_range accelRange, mpu9250_gyro_range gyroRange)
{
    uint8_t buff[3];
    uint8_t data[7];

    uint8_t accelConfig;
    uint8_t gyroConfig;

    // starting the I2C bus
    i2c_t3(_bus).begin(I2C_MASTER, 0, _pins, _pullups, I2C_RATE);

    // enable I2C master mode
    if (!writeRegister(USER_CTRL, I2C_MST_EN))
    {
        return -1;
    }

    // set the I2C bus speed to 400 kHz
    if (!writeRegister(I2C_MST_CTRL, I2C_MST_CLK))
    {
        return -1;
    }

    // set AK8963 to Power Down
    if (!writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN))
    {
        return -1;
    }

    // reset the MPU9250
    writeRegister(PWR_MGMNT_1, PWR_RESET);

    // wait for MPU-9250 to come back up
    delay(1);

    // reset the AK8963
    writeAK8963Register(AK8963_CNTL2, AK8963_RESET);

    // select clock source to gyro
    if (!writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL))
    {
        return -1;
    }

    // check the WHO AM I byte, expected value is 0x71 (decimal 113)
    if (whoAmI() != 0x71)
    {
        return -1;
    }

    // enable accelerometer and gyro
    if (!writeRegister(PWR_MGMNT_2, SEN_ENABLE))
    {
        return -1;
    }

    // setup the accel and gyro ranges
    switch (accelRange)
    {
        case ACCEL_RANGE_2G:
            // setting the accel range to 2G
            _accelScale = G * 2.0f/32767.5f;
            accelConfig = ACCEL_FS_SEL_2G;
            break;

        case ACCEL_RANGE_4G:
            // setting the accel range to 4G
            _accelScale = G * 4.0f/32767.5f;
            accelConfig = ACCEL_FS_SEL_4G;
            break;

        case ACCEL_RANGE_8G:
            // setting the accel range to 8G
            _accelScale = G * 8.0f/32767.5f;
            accelConfig = ACCEL_FS_SEL_8G;
            break;

        case ACCEL_RANGE_16G:
            // setting the accel range to 16G
            _accelScale = G * 16.0f/32767.5f;
            accelConfig = ACCEL_FS_SEL_16G;
            break;

        default:
            return -1;
    }

    if (!writeRegister(ACCEL_CONFIG, accelConfig))
    {
        return -1;
    }

    switch (gyroRange)
    {
        case GYRO_RANGE_250DPS:
            // setting the gyro range to 250DPS
            _gyroScale = 250.0f/32767.5f * DEG2RAD;
            gyroConfig = GYRO_FS_SEL_250DPS;
            break;

        case GYRO_RANGE_500DPS:
            // setting the gyro range to 500DPS
            _gyroScale = 500.0f/32767.5f * DEG2RAD;
            gyroConfig = GYRO_FS_SEL_500DPS;
            break;

        case GYRO_RANGE_1000DPS:
            // setting the gyro range to 1000DPS
            _gyroScale = 1000.0f/32767.5f * DEG2RAD;
            gyroConfig = GYRO_FS_SEL_1000DPS;
            break;

        case GYRO_RANGE_2000DPS:
            // setting the gyro range to 2000DPS
            _gyroScale = 2000.0f/32767.5f * DEG2RAD;
            gyroConfig = GYRO_FS_SEL_2000DPS;
            break;

        default:
            return -1;
    }

    if (!writeRegister(GYRO_CONFIG, gyroConfig))
    {
        return -1;
    }

    // enable I2C master mode
    if (!writeRegister(USER_CTRL, I2C_MST_EN))
    {
        return -1;
    }

    // set the I2C bus speed to 400 kHz
    if (!writeRegister(I2C_MST_CTRL, I2C_MST_CLK))
    {
        return -1;
    }

    // check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
    if (whoAmIAK8963() != 0x48)
    {
        return -1;
    }

    /* get the magnetometer calibration */

    // set AK8963 to Power Down
    if (!writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN))
    {
        return -1;
    }
    delay(100); // long wait between AK8963 mode changes

    // set AK8963 to FUSE ROM access
    if (!writeAK8963Register(AK8963_CNTL1, AK8963_FUSE_ROM))
    {
        return -1;
    }
    delay(100); // long wait between AK8963 mode changes

    // read the AK8963 ASA registers and compute magnetometer scale factors
    readAK8963Registers(AK8963_ASA, sizeof(buff), &buff[0]);
    _magScaleX = ((((float)buff[0]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
    _magScaleY = ((((float)buff[1]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
    _magScaleZ = ((((float)buff[2]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla

    // set AK8963 to Power Down
    if (!writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN))
    {
        return -1;
    }
    delay(100); // long wait between AK8963 mode changes

    // set AK8963 to 16 bit resolution, 100 Hz update rate
    if (!writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS2))
    {
        return -1;
    }
    delay(100); // long wait between AK8963 mode changes

    // select clock source to gyro
    if (!writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL))
    {
        return -1;
    }

    // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
    readAK8963Registers(AK8963_HXL, sizeof(data), &data[0]);

    // reset timing
    update = 0L;
    delta = 0.0f;

    // successful init, return 0
    return 0;
}


/* sets the DLPF and interrupt settings */
int MPU9250::setFilter(mpu9250_dlpf_bandwidth bandwidth, uint8_t srd)
{
    uint8_t data[7];

    uint8_t accelBandwidth;
    uint8_t gyroBandwidth;

    switch (bandwidth)
    {
        case DLPF_BANDWIDTH_184HZ:
            accelBandwidth = ACCEL_DLPF_184;
            gyroBandwidth = GYRO_DLPF_184;
            break;

        case DLPF_BANDWIDTH_92HZ:
            accelBandwidth = ACCEL_DLPF_92;
            gyroBandwidth = GYRO_DLPF_92;
            break;

        case DLPF_BANDWIDTH_41HZ:
            accelBandwidth = ACCEL_DLPF_41;
            gyroBandwidth = GYRO_DLPF_41;
            break;

        case DLPF_BANDWIDTH_20HZ:
            accelBandwidth = ACCEL_DLPF_20;
            gyroBandwidth = GYRO_DLPF_20;
            break;

        case DLPF_BANDWIDTH_10HZ:
            accelBandwidth = ACCEL_DLPF_5;
            gyroBandwidth = GYRO_DLPF_5;
            break;

        case DLPF_BANDWIDTH_5HZ:
            accelBandwidth = ACCEL_DLPF_20;
            gyroBandwidth = GYRO_DLPF_20;
            break;

        default:
            return -1;
    }

    // setting accel bandwidth
    if (!writeRegister(ACCEL_CONFIG2, accelBandwidth))
    {
        return -1;
    }

    // setting gyro bandwidth
    if (!writeRegister(CONFIG, gyroBandwidth))
    {
        return -1;
    }

    // setting the sample rate divider
    if (!writeRegister(SMPDIV, srd))
    {
        return -1;
    }

    if (srd > 9)
    {

        // set AK8963 to Power Down
        if (!writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN))
        {
            return -1;
        }
        delay(100); // long wait between AK8963 mode changes

        // set AK8963 to 16 bit resolution, 8 Hz update rate
        if (!writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS1))
        {
            return -1;
        }
        delay(100); // long wait between AK8963 mode changes

        // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
        readAK8963Registers(AK8963_HXL, sizeof(data), &data[0]);
    }

    // setup interrupt, 50 us pulse
    if (!writeRegister(INT_PIN_CFG, INT_PULSE_50US))
    { //
        return -1;
    }

    if (!writeRegister(INT_ENABLE, INT_RAW_RDY_EN))
    { // set to data ready
        return -1;
    }

    // successful filter setup, return 0
    return 0;
}

/* get accelerometer, gyro, magnetometer, and temperature data given pointers to store values, return data as counts */
void MPU9250::getMotion10Counts(int16_t* res)
{
    uint8_t buff[21];
    // this will also read AK8963_ST2 0x09 - Data overflow bit 3 and data read error status bit 2
    // via EXT_SENS_DATA_07 0x50 register
    // not checking it now, but probably should

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    // Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
    // the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
    // We have to make some allowance for this orientation mismatch in feeding the output to the quaternion filter.
    // For the MPU9250+MS5637 Mini breakout the +x accel/gyro is North, then -y accel/gyro is East. So if we want te quaternions properly aligned
    // we need to feed into the Madgwick function Ax, -Ay, -Az, Gx, -Gy, -Gz, My, -Mx, and Mz. But because gravity is by convention
    // positive down, we need to invert the accel data, so we pass -Ax, Ay, Az, Gx, -Gy, -Gz, My, -Mx, and Mz into the Madgwick
    // function to get North along the accel +x-axis, East along the accel -y-axis, and Down along the accel -z-axis.
    // This orientation choice can be modified to allow any convenient (non-NED) orientation convention.

    // combine into 16 bit values
    // accel/gyro is arranged HIGH, then LOW
    res[AX] = -((((int16_t)buff[0]) << 8) | buff[1]);
    res[AY] =  ((((int16_t)buff[2]) << 8) | buff[3]);
    res[AZ] =  ((((int16_t)buff[4]) << 8) | buff[5]);

    res[GX] =  ((((int16_t)buff[8]) << 8) | buff[9]);
    res[GY] = -((((int16_t)buff[10]) << 8) | buff[11]);
    res[GZ] = -((((int16_t)buff[12]) << 8) | buff[13]);

    // note exchange of indices 7 (hy) and 6 (hx)
    // magnetometer is arranged LOW, then HIGH
    res[MY] =  ((((int16_t)buff[15]) << 8) | buff[14]);
    res[MX] = -((((int16_t)buff[17]) << 8) | buff[16]);
    res[MZ] =  ((((int16_t)buff[19]) << 8) | buff[18]);

    res[TEMPERATURE] = ((((int16_t)buff[6]) << 8) | buff[7]);

    float us = micros();
    delta = (float)(us - update) * 0.000001; // uS -> full seconds
    update = us;
}

void MPU9250::getMotion10(float* res)
{
    int16_t buff[10];

    getMotion10Counts(buff);

    // typecast and scale to values
    res[AX] = ((float) buff[AX]) * _accelScale;
    res[AY] = ((float) buff[AY]) * _accelScale;
    res[AZ] = ((float) buff[AZ]) * _accelScale;

    res[GX] = ((float) buff[GX]) * _gyroScale;
    res[GY] = ((float) buff[GY]) * _gyroScale;
    res[GZ] = ((float) buff[GZ]) * _gyroScale;

    res[MX] = ((float) buff[MX]) * _magScaleY; // y sacle - see XY swap
    res[MY] = ((float) buff[MY]) * _magScaleX; // x scale - see XY swap
    res[MZ] = ((float) buff[MZ]) * _magScaleZ;

    res[TEMPERATURE] = ((((float) buff[TEMPERATURE]) - TEMPERATURE_OFFSET) * TEMPERATURE_SCALE) + TEMPERATURE_OFFSET; // t
}

void MPU9250::getMotion10Unbiased(float* res)
{
    getMotion10(res);

    // typecast and scale to values
    res[AX] -= bias[AX];
    res[AY] -= bias[AY];
    res[AZ] -= bias[AZ];

    res[GX] -= bias[GX];
    res[GY] -= bias[GY];
    res[GZ] -= bias[GZ];

    float cmx = res[MX] - bias[MX];
    float cmy = res[MY] - bias[MY];
    float cmz = res[MZ] - bias[MZ];

    res[MX] = cmx*bias[MXX] + cmy*bias[MXY] + cmz*bias[MXZ];
    res[MY] = cmx*bias[MYX] + cmy*bias[MYY] + cmz*bias[MYZ];
    res[MZ] = cmx*bias[MZX] + cmy*bias[MZY] + cmz*bias[MZZ];

    // no bias for temperature
}

void MPU9250::setBias(float* b)
{
    for (int i=0; i<18; i++)
    {
        bias[i] = b[i];
    }
}

/* writes a byte to MPU9250 register given a register address and data */
bool MPU9250::writeRegister(uint8_t subAddress, uint8_t data)
{
    uint8_t buff[1];

    i2c_t3(_bus).beginTransmission(_address); // open the device
    i2c_t3(_bus).write(subAddress); // write the register address
    i2c_t3(_bus).write(data); // write the data
    i2c_t3(_bus).endTransmission();
    delay(10); // need to slow down how fast I write to MPU9250

    // read back the register
    readRegisters(subAddress, sizeof(buff), &buff[0]);

    // check the read back register against the written register
    if (buff[0] == data) {
        return true;
    }
    else
    {
        return false;
    }
}

/* reads registers from MPU9250 given a starting register address, number of bytes, and a pointer to store data */
void MPU9250::readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    i2c_t3(_bus).beginTransmission(_address); // open the device
    i2c_t3(_bus).write(subAddress); // specify the starting register address
    i2c_t3(_bus).endTransmission(false);

    i2c_t3(_bus).requestFrom(_address, count); // specify the number of bytes to receive

    while (i2c_t3(_bus).available())
    {
        *dest++ = i2c_t3(_bus).readByte();
    }
}

/* writes a register to the AK8963 given a register address and data */
bool MPU9250::writeAK8963Register(uint8_t subAddress, uint8_t data)
{
    uint8_t buff[1];

    writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR); // set slave 0 to the AK8963 and set for write
    writeRegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeRegister(I2C_SLV0_DO, data); // store the data for write
    writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | 1); // enable I2C and send 1 byte

    // read the register and confirm
    readAK8963Registers(subAddress, sizeof(buff), &buff[0]);

    if (buff[0] == data)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* reads registers from the AK8963 */
void MPU9250::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR | I2C_READ_FLAG); // set slave 0 to the AK8963 and set for read
    writeRegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count); // enable I2C and request the bytes
    delayMicroseconds(100); // takes some time for these registers to fill
    readRegisters(EXT_SENS_DATA_00, count, dest); // read the bytes off the MPU9250 EXT_SENS_DATA registers
}

/* gets the MPU9250 WHO_AM_I register value, expected to be 0x71 */
uint8_t MPU9250::whoAmI()
{
    uint8_t buff[1];

    // read the WHO AM I register
    readRegisters(WHO_AM_I, sizeof(buff), &buff[0]);

    // return the register value
    return buff[0];
}

/* gets the AK8963 WHO_AM_I register value, expected to be 0x48 */
uint8_t MPU9250::whoAmIAK8963()
{
    uint8_t buff[1];

    // read the WHO AM I register
    readAK8963Registers(AK8963_WHO_AM_I, sizeof(buff), &buff[0]);

    // return the register value
    return buff[0];
}
