// LED pins
#define LED_PIN 13
#define RED_LED_PIN 17
#define YELLOW_LED_PIN 16
#define GREEN_LED_PIN 15
#define LEFT_BLUE_LED_PIN 14
#define RIGHT_BLUE_LED_PIN 2

#define LED_PULSE 200

// servo PWM pins
#define DRIVE_PIN 5
#define STEER_PIN 30

// button pin
#define BUTTON_PIN 8

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

// constants
#define G 9.807f
#define DEG2RAD (3.14159265359f/180.0f)
#define RAD2DEG (180.0f/3.14159265359f)
