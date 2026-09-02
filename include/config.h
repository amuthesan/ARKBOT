#pragma once

#include <Arduino.h>

// ==========================================
// Project Branding & Metadata
// ==========================================
#define ROBOT_NAME        "ARK-BOT"
#define ROBOT_VERSION     "v2.0"

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

// I2C Addresses
#define OLED_I2C_ADDRESS    0x3C  // 0.96" SSD1306 OLED (128x64)
#define PCA9685_I2C_ADDRESS 0x40  // PCA9685 16-Channel PWM Servo Driver

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
// 4 Legs: 0: Front-Right (FR), 1: Front-Left (FL), 2: Rear-Right (RR), 3: Rear-Left (RL)
// 3 Joints per leg: 0: Coxa/Hip, 1: Femur/Thigh, 2: Tibia/Calf
//
// Mapped to PCA9685 channels directly corresponding to original Arduino pins (D2..D13):
// - Front-Right (FR): Coxa D4  (CH 4),  Femur D2  (CH 2),  Tibia D3  (CH 3)
// - Front-Left  (FL): Coxa D10 (CH 10), Femur D8  (CH 8),  Tibia D9  (CH 9)
// - Rear-Right  (RR): Coxa D7  (CH 7),  Femur D5  (CH 5),  Tibia D6  (CH 6)
// - Rear-Left   (RL): Coxa D12 (CH 12), Femur D11 (CH 11), Tibia D13 (CH 13)
const int SERVO_CHANNELS[NUM_LEGS][SERVOS_PER_LEG] = {
    {4, 2, 3},    // Leg 0: Front-Right (Coxa D4, Femur D2, Tibia D3)
    {10, 8, 9},   // Leg 1: Front-Left  (Coxa D10, Femur D8, Tibia D9)
    {7, 5, 6},    // Leg 2: Rear-Right  (Coxa D7, Femur D5, Tibia D6)
    {12, 11, 13}  // Leg 3: Rear-Left   (Coxa D12, Femur D11, Tibia D13)
};

// Leg names for display and UI
const char* const LEG_NAMES[NUM_LEGS] = {
    "FR", // Front-Right
    "FL", // Front-Left
    "RR", // Rear-Right
    "RL"  // Rear-Left
};

const char* const LEG_FULL_NAMES[NUM_LEGS] = {
    "Front-Right",
    "Front-Left",
    "Rear-Right",
    "Rear-Left"
};

const char* const JOINT_NAMES[SERVOS_PER_LEG] = {
    "Coxa",
    "Femur",
    "Tibia"
};
