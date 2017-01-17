// LED pins
#define LED_PIN 13
#define RED_LED_PIN 17
#define YELLOW_LED_PIN 16
#define GREEN_LED_PIN 15
#define LEFT_BLUE_LED_PIN 14
#define RIGHT_BLUE_LED_PIN 2

#define LED_PULSE 250

// servo PWM pins
#define DRIVE_PIN 5
#define STEER_PIN 30

// steering
#define STEER_ZERO 1500
#define STEER_MIN 1160 // left
#define STEER_MAX 1840 // right

#define EEPROM_STEER_ZERO 10
#define EEPROM_STEER_MIN 12
#define EEPROM_STEER_MAX 14

// drive
#define DRIVE_ZERO 1500
#define DRIVE_FORWARD 1550
#define DRIVE_BACKWARD 1450
#define DRIVE_MIN 1400
#define DRIVE_MAX 1600
#define DRIVE_STEP 20
#define DRIVE_INTERVAL 50

#define EEPROM_DRIVE_ZERO 20
#define EEPROM_DRIVE_MIN 22
#define EEPROM_DRIVE_MAX 24
#define EEPROM_DRIVE_INTERVAL 26

// button pin
#define BUTTON_PIN 8

#define BUTTON_NONE 0
#define BUTTON_PRESS 1
#define BUTTON_RELEASE 2

#define DEBOUNCE_DELAY 50 // ms

// sonar pins
#define SONAR1_TRIG_PIN 28
#define SONAR1_ECHO_PIN 29
#define SONAR2_TRIG_PIN 26
#define SONAR2_ECHO_PIN 27
#define SONAR3_TRIG_PIN 24
#define SONAR3_ECHO_PIN 25
#define SONAR4_TRIG_PIN 11
#define SONAR4_ECHO_PIN 12
#define SONAR5_TRIG_PIN 9
#define SONAR5_ECHO_PIN 10

#define SONAR_NUM 5 // Number or sensors
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo)

// ESC I2C - bus 2 - pins 3/4

// ESC serial - serial 1 - pins 0/1

// BT serial - serial 4 - pins 31/32

// MPU-9250 IMU I2C - bus 0 - pins 18/19
#define IMU_INT_PIN 22
#define IMU_INTERVAL 50 // ms

// IMU data indices
#define AX 0
#define AY 1
#define AZ 2

#define GX 3
#define GY 4
#define GZ 5

#define MX 6
#define MY 7
#define MZ 8

#define TEMPERATURE 9

// quaternion indices
#define QW 0
#define QX 1
#define QY 2
#define QZ 3

// IMU constants
#define G 9.807f
#define DEG2RAD (3.14159265359f/180.0f)
#define RAD2DEG (180.0f/3.14159265359f)

// log levels
#define LOG_INFO 0
#define LOG_WARNING 1
#define LOG_ERROR 2

#define EEPROM_LOG_ID 0

// modes
#define MODE_IDLE 0 // do nothing
#define MODE_BLINK 1 // blink LEDs
#define MODE_READY 2 // enter countdown on button-press
#define MODE_COUNTDOWN 3 // 5s countdown to run
#define MODE_RUN 4 // run
#define MODE_SONAR 5 // circular sonar sweep
#define MODE_IMU 6 // dump IMU data

#define COUNTDOWN_INTERVAL 5000 // ms = 5s
#define READY_TIMEOUT 15000 // ms = 15s

// serial buffer size
#define SERIAL_BUFFER 128
#define SERIAL_END1 10
#define SERIAL_END2 13

// eeprom actions
#define EEPROM_READ 0
#define EEPROM_WRITE 1
#define EEPROM_RESET 2
#define EEPROM_SHOW 3

