#pragma once

#include <Arduino.h>

// ==========================================
// Project Branding & Metadata
// ==========================================
#define ROBOT_NAME        "ARK-BOT"
#define ROBOT_VERSION     "v1.0.6"



// ==========================================
// Quadruped Physical & Kinematics Dimensions (mm)
// ==========================================
const float LENGTH_A    = 84.0f;    // Femur (Thigh) length
const float LENGTH_B    = 145.0f;   // Tibia (Calf) length
const float LENGTH_C    = 72.5f;    // Coxa (Hip) length
const float LENGTH_SIDE = 145.4f;   // Body chassis base width/length

// Movement & Stance Z Coordinates
const float Z_ABSOLUTE  = -56.0f;   // Mechanical rest/sit height
const float Z_DEFAULT   = -100.0f;  // Normal standing height
const float Z_HIGH      = -130.0f;  // High/tall standing height
const float Z_UP        = -60.0f;   // Swing phase leg lift clearance
const float Z_BOOT      = Z_ABSOLUTE;

// Locomotion Walking Height Presets
const float Z_WALK_LOW    = -80.0f;   // Low / Crouch walking height
const float Z_UP_LOW      = -50.0f;   // Low walking swing lift

const float Z_WALK_NORMAL = -100.0f;  // Normal walking height
const float Z_UP_NORMAL   = -60.0f;   // Normal walking swing lift

const float Z_WALK_HIGH   = -125.0f;  // High / Tall walking height
const float Z_UP_HIGH     = -85.0f;   // High walking swing lift

const float X_DEFAULT   = 124.0f;   // Default neutral X foot spread
const float X_OFFSET    = 0.0f;
const float Y_START     = 0.0f;
const float Y_STEP      = 80.0f;    // Stride length per gait cycle

// Speed Presets
const float SPOT_TURN_SPEED   = 5.0f;
const float LEG_MOVE_SPEED    = 10.0f;
const float BODY_MOVE_SPEED   = 4.0f;
const float STAND_SEAT_SPEED  = 2.0f;


// ==========================================
// Wi-Fi Configuration (Home Station + SoftAP Fallback)
// ==========================================
#define WIFI_STA_SSID      "AKR Home"
#define WIFI_STA_PASS      "brandy78755862"
#define MDNS_HOSTNAME      "arkbot"     // Accessible via http://arkbot.local

#define WIFI_AP_SSID       "ARK-BOT-AP"
#define WIFI_AP_PASS       "12345678"   // Minimum 8 characters
#define WIFI_AP_CHANNEL    1
#define WIFI_MAX_CONN      4

// ==========================================
// Seeed Studio XIAO ESP32-C6 RF Antenna Control
// ==========================================
// GPIO 3: RF Switch Power Enable (active LOW to activate switch circuit)
// GPIO 14: Antenna Selector (LOW = Internal Ceramic Antenna, HIGH = External U.FL/IPEX)
#define ANT_PWR_PIN        3
#define ANT_SEL_PIN        14
#define DEFAULT_EXT_ANT    false

// ==========================================
// Hardware Pin & I2C Definitions
// ==========================================

// Seeed Studio Expansion Board Buzzer Pin (D3 -> GPIO 21 on XIAO ESP32C6)
#if defined(D3)
#define BUZZER_PIN D3
#elif defined(A3)
#define BUZZER_PIN A3
#else
#define BUZZER_PIN 21
#endif

// Seeed Studio Expansion Board Battery Voltage Sense Pin (A0 -> GPIO 0 on XIAO ESP32C6)
// Connected across 10k / 2.2k voltage divider
#if defined(A0)
#define BATTERY_ADC_PIN A0
#elif defined(D0)
#define BATTERY_ADC_PIN D0
#else
#define BATTERY_ADC_PIN 0
#endif

// Default Voltage Divider Calibration Multiplier:
// User Measured: 10.0V source gives 1.974V across divider -> 10.0 / 1.974 = 5.065856f
#define DEFAULT_BATTERY_MULTIPLIER   5.06586f
#define BATTERY_LOW_VOLTAGE_ALERT    6.6f      // Low voltage threshold alert in Volts (e.g. 2S/3S)
#define NVS_KEY_BAT_MULT             "bat_mult"


// I2C Addresses & IMU Configuration
#define OLED_I2C_ADDRESS        0x3C  // 0.96" SSD1306 OLED (128x64)
#define PCA9685_I2C_ADDRESS     0x40  // PCA9685 16-Channel PWM Servo Driver
#define MPU6050_I2C_ADDRESS     0x68  // MPU6050 6-Axis IMU (Primary)
#define MPU6050_I2C_ALT_ADDRESS 0x69  // MPU6050 6-Axis IMU (Alternate AD0=High)
#define IMU_FILTER_ALPHA        0.98f // Complementary filter weight (98% Gyro + 2% Accel)
#define IMU_MOUNT_YAW_180       true  // IMU mounted with 180 deg yaw rotation relative to forward body frame


// OLED Display Configuration
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// ==========================================
// Servo & PCA9685 Configuration
// ==========================================
#define SERVO_FREQ 50  // Analog/standard servos run at 50Hz (20ms period)

// 12-bit PCA9685 pulse counts (0 to 4095) for 50Hz (20,000 µs period)
#define SERVOMIN_PULSE_US  500
#define SERVOMAX_PULSE_US  2500
#define SERVOMIN_COUNT     102   // Pulse length for 0 degrees
#define SERVOMAX_COUNT     512   // Pulse length for 180 degrees

#define DEFAULT_CALIBRATION_ANGLE 90

// Number of legs and servos per leg
#define NUM_LEGS        4
#define SERVOS_PER_LEG  3

// ==========================================
// PCA9685 Channel Mapping for Quadruped
// ==========================================
// 4 Legs: 0: Front-Right (FR), 1: Rear-Right (RR), 2: Front-Left (FL), 3: Rear-Left (RL)
// 3 Joints per leg: 0: Coxa/Hip, 1: Femur/Thigh, 2: Tibia/Calf
//
// PCA9685 PWM Channel Assignments (Channels 2..13):
// - Leg 0 (FR): Coxa (CH 4),  Femur (CH 2),  Tibia (CH 3)
// - Leg 1 (RR): Coxa (CH 7),  Femur (CH 5),  Tibia (CH 6)
// - Leg 2 (FL): Coxa (CH 10), Femur (CH 8),  Tibia (CH 9)
// - Leg 3 (RL): Coxa (CH 13), Femur (CH 11), Tibia (CH 12)
const int SERVO_CHANNELS[NUM_LEGS][SERVOS_PER_LEG] = {
    {4, 2, 3},    // Leg 0: Front-Right (Coxa CH4, Femur CH2, Tibia CH3)
    {7, 5, 6},    // Leg 1: Rear-Right  (Coxa CH7, Femur CH5, Tibia CH6)
    {10, 8, 9},   // Leg 2: Front-Left  (Coxa CH10, Femur CH8, Tibia CH9)
    {13, 11, 12}  // Leg 3: Rear-Left   (Coxa CH13, Femur CH11, Tibia CH12)
};

// Leg names for display and UI
const char* const LEG_NAMES[NUM_LEGS] = {
    "FR", // Front-Right
    "RR", // Rear-Right
    "FL", // Front-Left
    "RL"  // Rear-Left
};

const char* const LEG_FULL_NAMES[NUM_LEGS] = {
    "Front-Right",
    "Rear-Right",
    "Front-Left",
    "Rear-Left"
};

const char* const JOINT_NAMES[SERVOS_PER_LEG] = {
    "Coxa",
    "Femur",
    "Tibia"
};
