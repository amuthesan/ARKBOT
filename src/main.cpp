#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include <ESPmDNS.h>

#include "config.h"
#include "web_page.h"

// ==========================================
// Hardware & Subsystem Instances
// ==========================================
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_I2C_ADDRESS);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);
Preferences preferences;

// ==========================================
// Original Arduino Nano Kinematics Constants
// ==========================================
const float length_a = 84.0f;    // Femur
const float length_b = 145.0f;   // Tibia
const float length_c = 72.5f;    // Coxa
const float length_side = 145.4f;// Base size
const float z_absolute = -56.0f;

const float z_default = -100.0f;
const float z_high = -130.0f;
const float z_up = -60.0f;
const float z_boot = z_absolute;
const float x_default = 124.0f;
const float x_offset = 0.0f;
const float y_start = 0.0f;
const float y_step = 80.0f;

const float spot_turn_speed = 5.0f;
const float leg_move_speed = 10.0f;
const float body_move_speed = 4.0f;
const float stand_seat_speed = 2.0f;

const float KEEP = 255.0f;
const float pi = 3.14159265358979323846f;

// Precalculated turn constants
const float temp_a = sqrt(pow(2 * x_default + length_side, 2) + pow(y_step, 2));
const float temp_b = 2 * (y_start + y_step) + length_side;
const float temp_c = sqrt(pow(2 * x_default + length_side, 2) + pow(2 * y_start + y_step + length_side, 2));
const float temp_alpha = acos((pow(temp_a, 2) + pow(temp_b, 2) - pow(temp_c, 2)) / 2 / temp_a / temp_b);

const float turn_x1 = (temp_a - length_side) / 2;
const float turn_y1 = y_start + y_step / 2;
const float turn_x0 = turn_x1 - temp_b * cos(temp_alpha);
const float turn_y0 = temp_b * sin(temp_alpha) - turn_y1 - length_side;

// 1-to-1 Nano pin to PCA9685 port mapping:
// servo_pin[4][3] = { {2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13} }
const int servo_pin[4][3] = {
    {2, 3, 4},     // Leg 0 (FR)
    {5, 6, 7},     // Leg 1 (RR)
    {8, 9, 10},    // Leg 2 (FL)
    {11, 12, 13}   // Leg 3 (RL)
};

// Real-time coordinates & speed
volatile float site_now[4][3];
volatile float site_expect[4][3];
float temp_speed[4][3];
float move_speed = stand_seat_speed;
float speed_multiple = 1.0f;
int rest_counter = 0;

// State Tracking
int currentAngles[NUM_LEGS][SERVOS_PER_LEG];
bool servoEnabled[NUM_LEGS][SERVOS_PER_LEG];
bool oledReady = false;
bool pcaReady = false;
bool wifiStaConnected = false;
bool useExternalAntenna = false;
String activeIp = "192.168.4.1";
String currentMode = "ALL @ 90deg";

// Kinematics active flag: ONLY true when running an action (Stand, Sit, Walk, etc.)
// When in Calibration mode, this is false so calibration 90° and sliders are NEVER overwritten!
volatile bool kinematicsActive = false;
volatile bool isActionRunning = false;
volatile bool stopRequested = false;

// Persistent Wi-Fi Credentials
String staSsid = WIFI_STA_SSID;
String staPass = WIFI_STA_PASS;

// Task Handles
TaskHandle_t kinematicsTaskHandle = NULL;
TaskHandle_t actionTaskHandle = NULL;

// Forward Declarations
void playStepChime();
void playReadyChime();
void playBootChime();
void playErrorChime();
void setAntenna(bool external, bool persist);
void setServoAngle(int channel, int angle);
void cartesian_to_polar(float &alpha, float &beta, float &gamma, float x, float y, float z);
void polar_to_servo(int leg, float alpha, float beta, float gamma);
void set_site(int leg, float x, float y, float z);
void wait_reach(int leg);
void wait_all_reach();
bool is_stand();
void stand();
void sit();
void step_forward(unsigned int step);
void step_back(unsigned int step);
void turn_left(unsigned int step);
void turn_right(unsigned int step);
void body_left(int i);
void body_right(int i);
void hand_wave(int i);
void hand_shake(int i);
void executeAction(const String& act, int steps, float spd);
void parseSerialCommand(const String& cmd);
void calibrateAllServos(bool sequential);
void drawMainDashboard(bool heartbeat, int cycleIndex);
void drawSplashScreen();

// ==========================================
// Seeed Studio XIAO ESP32-C6 Antenna Control
// ==========================================
void initAntenna() {
    preferences.begin("ark_rf", false);
    useExternalAntenna = preferences.getBool("ext_ant", DEFAULT_EXT_ANT);
    preferences.end();
    setAntenna(useExternalAntenna, false);
}

void setAntenna(bool external, bool persist = true) {
    pinMode(ANT_PWR_PIN, OUTPUT);
    digitalWrite(ANT_PWR_PIN, LOW);
    delay(10);

    pinMode(ANT_SEL_PIN, OUTPUT);
    digitalWrite(ANT_SEL_PIN, external ? HIGH : LOW);
    useExternalAntenna = external;

    if (persist) {
        preferences.begin("ark_rf", false);
        preferences.putBool("ext_ant", external);
        preferences.end();
    }

    Serial.printf("[RF] Antenna: %s\n", external ? "EXTERNAL (IPEX)" : "INTERNAL (Ceramic)");
}

// ==========================================
// Buzzer Audio Feedback (LEDC PWM)
// ==========================================
void initBuzzer() {
    ledcAttach(BUZZER_PIN, 2000, 8);
    ledcWriteTone(BUZZER_PIN, 0);
}

void playTone(int freq, int durationMs) {
    if (freq <= 0) {
        ledcWriteTone(BUZZER_PIN, 0);
        delay(durationMs);
        return;
    }
    ledcWriteTone(BUZZER_PIN, freq);
    delay(durationMs);
    ledcWriteTone(BUZZER_PIN, 0);
}

void playBootChime() {
    playTone(1047, 70); delay(30);
    playTone(1319, 70); delay(30);
    playTone(1568, 70); delay(30);
    playTone(2093, 120);
}

void playStepChime() { playTone(1760, 25); }
void playReadyChime() { playTone(1319, 80); delay(40); playTone(1760, 160); }
void playErrorChime() { playTone(440, 150); delay(60); playTone(349, 250); }

// ==========================================
// I2C Helper & PCA9685 Control
// ==========================================
bool probeI2C(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}

int angleToPulse(int angle) {
    angle = constrain(angle, 0, 180);
    return map(angle, 0, 180, SERVOMIN_COUNT, SERVOMAX_COUNT);
}

void setServoAngle(int channel, int angle) {
    if (!pcaReady || channel < 0 || channel > 15) return;
    int pulse = angleToPulse(angle);
    pwm.setPWM(channel, 0, pulse);
}

// ==========================================
// Calibration & Manual Power Controls
// ==========================================
void calibrateAllServos(bool sequential) {
    kinematicsActive = false; // Disable kinematics solver from overwriting calibration!
    isActionRunning = false;
    currentMode = "ALL @ 90deg";

    for (int l = 0; l < NUM_LEGS; l++) {
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            currentAngles[l][j] = DEFAULT_CALIBRATION_ANGLE;
            servoEnabled[l][j] = true;
            int ch = SERVO_CHANNELS[l][j];
            setServoAngle(ch, DEFAULT_CALIBRATION_ANGLE);
            if (sequential) {
                playStepChime();
                delay(70);
            }
        }
    }
    playReadyChime();
    drawMainDashboard(true, 0);
    Serial.println(F("[CALIB] All 12 servos centered to 90 degrees."));
}

void setMasterPower(bool enable) {
    kinematicsActive = false;
    for (int l = 0; l < NUM_LEGS; l++) {
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            servoEnabled[l][j] = enable;
            int ch = SERVO_CHANNELS[l][j];
            if (enable) {
                setServoAngle(ch, currentAngles[l][j]);
            } else if (pcaReady) {
                pwm.setPWM(ch, 0, 0);
            }
        }
    }
    currentMode = enable ? "ALL ENERGIZED" : "ALL RELEASED";
    playStepChime();
}

void setServoPower(int leg, int joint, bool enable) {
    if (leg < 0 || leg >= NUM_LEGS || joint < 0 || joint >= SERVOS_PER_LEG) return;
    servoEnabled[leg][joint] = enable;
    int ch = SERVO_CHANNELS[leg][joint];
    if (enable) {
        setServoAngle(ch, currentAngles[leg][joint]);
    } else if (pcaReady) {
        pwm.setPWM(ch, 0, 0);
    }
}

void setLegPower(int leg, bool enable) {
    if (leg < 0 || leg >= NUM_LEGS) return;
    for (int j = 0; j < SERVOS_PER_LEG; j++) setServoPower(leg, j, enable);
    playStepChime();
}

// ==========================================
// Exact Original Arduino Nano Kinematics Implementation
// ==========================================
void initKinematicsSites() {
    set_site(0, x_default - x_offset, y_start + y_step, z_boot);
    set_site(1, x_default - x_offset, y_start + y_step, z_boot);
    set_site(2, x_default + x_offset, y_start, z_boot);
    set_site(3, x_default + x_offset, y_start, z_boot);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            site_now[i][j] = site_expect[i][j];
            temp_speed[i][j] = 0;
            currentAngles[i][j] = 90;
            servoEnabled[i][j] = true;
        }
    }
}

// Mathematical model 2/2: Cartesian to Polar
void cartesian_to_polar(float &alpha, float &beta, float &gamma, float x, float y, float z) {
    float v, w;
    w = (x >= 0 ? 1 : -1) * (sqrt(pow(x, 2) + pow(y, 2)));
    v = w - length_c;
    alpha = atan2(z, v) + acos((pow(length_a, 2) - pow(length_b, 2) + pow(v, 2) + pow(z, 2)) / 2 / length_a / sqrt(pow(v, 2) + pow(z, 2)));
    beta = acos((pow(length_a, 2) + pow(length_b, 2) - pow(v, 2) - pow(z, 2)) / 2 / length_a / length_b);
    gamma = (w >= 0) ? atan2(y, x) : atan2(-y, -x);

    alpha = alpha / pi * 180.0f;
    beta = beta / pi * 180.0f;
    gamma = gamma / pi * 180.0f;
}

// Map polar angles to servos
void polar_to_servo(int leg, float alpha, float beta, float gamma) {
    if (leg == 0) {
        alpha = 90.0f - alpha;
        beta = beta;
        gamma += 90.0f;
    } else if (leg == 1) {
        alpha += 90.0f;
        beta = 180.0f - beta;
        gamma = 90.0f - gamma;
    } else if (leg == 2) {
        alpha += 90.0f;
        beta = 180.0f - beta;
        gamma = 90.0f - gamma;
    } else if (leg == 3) {
        alpha = 90.0f - alpha;
        beta = beta;
        gamma += 90.0f;
    }

    int a = constrain((int)round(alpha), 0, 180);
    int b = constrain((int)round(beta), 0, 180);
    int g = constrain((int)round(gamma), 0, 180);

    // Joint 0: Coxa (g), Joint 1: Femur (a), Joint 2: Tibia (b)
    currentAngles[leg][0] = g;
    currentAngles[leg][1] = a;
    currentAngles[leg][2] = b;

    if (pcaReady) {
        setServoAngle(SERVO_CHANNELS[leg][0], g); // Coxa
        setServoAngle(SERVO_CHANNELS[leg][1], a); // Femur
        setServoAngle(SERVO_CHANNELS[leg][2], b); // Tibia
    }
}

void set_site(int leg, float x, float y, float z) {
    float length_x = 0, length_y = 0, length_z = 0;

    if (x != KEEP) length_x = x - site_now[leg][0];
    if (y != KEEP) length_y = y - site_now[leg][1];
    if (z != KEEP) length_z = z - site_now[leg][2];

    float length = sqrt(pow(length_x, 2) + pow(length_y, 2) + pow(length_z, 2));

    if (length > 0.001f) {
        temp_speed[leg][0] = length_x / length * move_speed * speed_multiple;
        temp_speed[leg][1] = length_y / length * move_speed * speed_multiple;
        temp_speed[leg][2] = length_z / length * move_speed * speed_multiple;
    } else {
        temp_speed[leg][0] = 0;
        temp_speed[leg][1] = 0;
        temp_speed[leg][2] = 0;
    }

    if (x != KEEP) site_expect[leg][0] = x;
    if (y != KEEP) site_expect[leg][1] = y;
    if (z != KEEP) site_expect[leg][2] = z;
}

void wait_reach(int leg) {
    while (!stopRequested) {
        if (fabs(site_now[leg][0] - site_expect[leg][0]) < 0.01f &&
            fabs(site_now[leg][1] - site_expect[leg][1]) < 0.01f &&
            fabs(site_now[leg][2] - site_expect[leg][2]) < 0.01f) {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void wait_all_reach() {
    for (int i = 0; i < 4; i++) {
        wait_reach(i);
        if (stopRequested) break;
    }
}

bool is_stand() {
    return (site_now[0][2] <= -70.0f);
}

// 50Hz FreeRTOS Kinematics Task (replaces Arduino FlexiTimer2)
void kinematicsTask(void* parameter) {
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // 20ms = 50Hz
    TickType_t xLastWakeTime = xTaskGetTickCount();

    float alpha, beta, gamma;

    for (;;) {
        if (kinematicsActive) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    if (fabs(site_now[i][j] - site_expect[i][j]) >= fabs(temp_speed[i][j])) {
                        site_now[i][j] += temp_speed[i][j];
                    } else {
                        site_now[i][j] = site_expect[i][j];
                    }
                }
                cartesian_to_polar(alpha, beta, gamma, site_now[i][0], site_now[i][1], site_now[i][2]);
                polar_to_servo(i, alpha, beta, gamma);
            }
            rest_counter = rest_counter + 1;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// ==========================================
// Exact Original Gait Routines
// ==========================================
// Walking Height State
float z_stance = Z_WALK_NORMAL;
float z_swing_up = Z_UP_NORMAL;
String currentWalkHeight = "normal";

void setWalkingHeight(const String& profile) {
    if (profile.equalsIgnoreCase("low") || profile.equalsIgnoreCase("crouch")) {
        currentWalkHeight = "low";
        z_stance = Z_WALK_LOW;
        z_swing_up = Z_UP_LOW;
        currentMode = "HEIGHT: LOW";
    } else if (profile.equalsIgnoreCase("high") || profile.equalsIgnoreCase("tall")) {
        currentWalkHeight = "high";
        z_stance = Z_WALK_HIGH;
        z_swing_up = Z_UP_HIGH;
        currentMode = "HEIGHT: HIGH";
    } else {
        currentWalkHeight = "normal";
        z_stance = Z_WALK_NORMAL;
        z_swing_up = Z_UP_NORMAL;
        currentMode = "HEIGHT: NORMAL";
    }

    if (is_stand()) {
        move_speed = stand_seat_speed;
        for (int leg = 0; leg < 4; leg++) {
            set_site(leg, KEEP, KEEP, z_stance);
        }
        wait_all_reach();
    }
}

// ==========================================
// Exact Original Gait Routines with Dynamic Stance
// ==========================================
void sit() {
    move_speed = stand_seat_speed;
    currentMode = "SITTING";
    for (int leg = 0; leg < 4; leg++) {
        set_site(leg, KEEP, KEEP, z_boot);
    }
    wait_all_reach();
    currentMode = "REST / SIT";
}

void stand() {
    move_speed = stand_seat_speed;
    currentMode = "STANDING";
    for (int leg = 0; leg < 4; leg++) {
        set_site(leg, KEEP, KEEP, z_stance);
    }
    wait_all_reach();
    currentMode = "STAND";
}

void stand_high(float target_z = Z_WALK_HIGH) {
    move_speed = stand_seat_speed;
    currentMode = "STAND HIGH";
    for (int leg = 0; leg < 4; leg++) {
        set_site(leg, KEEP, KEEP, target_z);
    }
    wait_all_reach();
    currentMode = "STAND HIGH";
}

void step_forward(unsigned int step) {
    move_speed = leg_move_speed;
    currentMode = "FORWARD";
    while (step-- > 0 && !stopRequested) {
        if (site_now[2][1] == y_start) {
            // leg 2&1 move
            set_site(2, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(2, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(2, x_default + x_offset, y_start + 2 * y_step, z_stance);
            wait_all_reach();

            move_speed = body_move_speed;
            set_site(0, x_default + x_offset, y_start, z_stance);
            set_site(1, x_default + x_offset, y_start + 2 * y_step, z_stance);
            set_site(2, x_default - x_offset, y_start + y_step, z_stance);
            set_site(3, x_default - x_offset, y_start + y_step, z_stance);
            wait_all_reach();

            move_speed = leg_move_speed;
            set_site(1, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(1, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(1, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        } else {
            // leg 0&3 move
            set_site(0, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(0, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(0, x_default + x_offset, y_start + 2 * y_step, z_stance);
            wait_all_reach();

            move_speed = body_move_speed;
            set_site(0, x_default - x_offset, y_start + y_step, z_stance);
            set_site(1, x_default - x_offset, y_start + y_step, z_stance);
            set_site(2, x_default + x_offset, y_start, z_stance);
            set_site(3, x_default + x_offset, y_start + 2 * y_step, z_stance);
            wait_all_reach();

            move_speed = leg_move_speed;
            set_site(3, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(3, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(3, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        }
    }
    if (!stopRequested) currentMode = "STAND";
}

void step_back(unsigned int step) {
    move_speed = leg_move_speed;
    currentMode = "BACKWARD";
    while (step-- > 0 && !stopRequested) {
        if (site_now[3][1] == y_start) {
            // leg 3&0 move
            set_site(3, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(3, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(3, x_default + x_offset, y_start + 2 * y_step, z_stance);
            wait_all_reach();

            move_speed = body_move_speed;
            set_site(0, x_default + x_offset, y_start + 2 * y_step, z_stance);
            set_site(1, x_default + x_offset, y_start, z_stance);
            set_site(2, x_default - x_offset, y_start + y_step, z_stance);
            set_site(3, x_default - x_offset, y_start + y_step, z_stance);
            wait_all_reach();

            move_speed = leg_move_speed;
            set_site(0, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(0, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(0, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        } else {
            // leg 1&2 move
            set_site(1, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(1, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(1, x_default + x_offset, y_start + 2 * y_step, z_stance);
            wait_all_reach();

            move_speed = body_move_speed;
            set_site(0, x_default - x_offset, y_start + y_step, z_stance);
            set_site(1, x_default - x_offset, y_start + y_step, z_stance);
            set_site(2, x_default + x_offset, y_start + 2 * y_step, z_stance);
            set_site(3, x_default + x_offset, y_start, z_stance);
            wait_all_reach();

            move_speed = leg_move_speed;
            set_site(2, x_default + x_offset, y_start + 2 * y_step, z_swing_up);
            wait_all_reach();
            set_site(2, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();
            set_site(2, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        }
    }
    if (!stopRequested) currentMode = "STAND";
}

void turn_left(unsigned int step) {
    move_speed = spot_turn_speed;
    currentMode = "TURN LEFT";
    while (step-- > 0 && !stopRequested) {
        if (site_now[3][1] == y_start) {
            // leg 3&1 move
            set_site(3, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();

            set_site(0, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(1, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(2, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(3, turn_x0 + x_offset, turn_y0, z_swing_up);
            wait_all_reach();

            set_site(3, turn_x0 + x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(0, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(1, turn_x0 + x_offset, turn_y0, z_stance);
            set_site(2, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(3, turn_x0 - x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(1, turn_x0 + x_offset, turn_y0, z_swing_up);
            wait_all_reach();

            set_site(0, x_default + x_offset, y_start, z_stance);
            set_site(1, x_default + x_offset, y_start, z_swing_up);
            set_site(2, x_default - x_offset, y_start + y_step, z_stance);
            set_site(3, x_default - x_offset, y_start + y_step, z_stance);
            wait_all_reach();

            set_site(1, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        } else {
            // leg 0&2 move
            set_site(0, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();

            set_site(0, turn_x0 + x_offset, turn_y0, z_swing_up);
            set_site(1, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(2, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(3, turn_x1 - x_offset, turn_y1, z_stance);
            wait_all_reach();

            set_site(0, turn_x0 + x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(0, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(1, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(2, turn_x0 + x_offset, turn_y0, z_stance);
            set_site(3, turn_x1 + x_offset, turn_y1, z_stance);
            wait_all_reach();

            set_site(2, turn_x0 + x_offset, turn_y0, z_swing_up);
            wait_all_reach();

            set_site(0, x_default - x_offset, y_start + y_step, z_stance);
            set_site(1, x_default - x_offset, y_start + y_step, z_stance);
            set_site(2, x_default + x_offset, y_start, z_swing_up);
            set_site(3, x_default + x_offset, y_start, z_stance);
            wait_all_reach();

            set_site(2, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        }
    }
    if (!stopRequested) currentMode = "STAND";
}

void turn_right(unsigned int step) {
    move_speed = spot_turn_speed;
    currentMode = "TURN RIGHT";
    while (step-- > 0 && !stopRequested) {
        if (site_now[2][1] == y_start) {
            // leg 2&0 move
            set_site(2, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();

            set_site(0, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(1, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(2, turn_x0 + x_offset, turn_y0, z_swing_up);
            set_site(3, turn_x1 + x_offset, turn_y1, z_stance);
            wait_all_reach();

            set_site(2, turn_x0 + x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(0, turn_x0 + x_offset, turn_y0, z_stance);
            set_site(1, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(2, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(3, turn_x1 - x_offset, turn_y1, z_stance);
            wait_all_reach();

            set_site(0, turn_x0 + x_offset, turn_y0, z_swing_up);
            wait_all_reach();

            set_site(0, x_default + x_offset, y_start, z_swing_up);
            set_site(1, x_default + x_offset, y_start, z_stance);
            set_site(2, x_default - x_offset, y_start + y_step, z_stance);
            set_site(3, x_default - x_offset, y_start + y_step, z_stance);
            wait_all_reach();

            set_site(0, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        } else {
            // leg 1&3 move
            set_site(1, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();

            set_site(0, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(1, turn_x0 + x_offset, turn_y0, z_swing_up);
            set_site(2, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(3, turn_x0 - x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(1, turn_x0 + x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(0, turn_x1 - x_offset, turn_y1, z_stance);
            set_site(1, turn_x0 - x_offset, turn_y0, z_stance);
            set_site(2, turn_x1 + x_offset, turn_y1, z_stance);
            set_site(3, turn_x0 + x_offset, turn_y0, z_stance);
            wait_all_reach();

            set_site(3, turn_x0 + x_offset, turn_y0, z_swing_up);
            wait_all_reach();

            set_site(0, x_default - x_offset, y_start + y_step, z_stance);
            set_site(1, x_default - x_offset, y_start + y_step, z_stance);
            set_site(2, x_default + x_offset, y_start, z_stance);
            set_site(3, x_default + x_offset, y_start, z_swing_up);
            wait_all_reach();

            set_site(3, x_default + x_offset, y_start, z_stance);
            wait_all_reach();
        }
    }
    if (!stopRequested) currentMode = "STAND";
}

void body_right(int i) {
    // move body to right
    set_site(0, site_now[0][0] + i * 20, KEEP, KEEP);
    set_site(1, site_now[1][0] + i * 20, KEEP, KEEP);
    set_site(2, site_now[2][0] - i * 20, KEEP, KEEP);
    set_site(3, site_now[3][0] - i * 20, KEEP, KEEP);
    wait_all_reach();
}

void body_left(int i) {
    set_site(0, site_now[0][0] - i * 20, KEEP, KEEP);
    set_site(1, site_now[1][0] - i * 20, KEEP, KEEP);
    set_site(2, site_now[2][0] + i * 20, KEEP, KEEP);
    set_site(3, site_now[3][0] + i * 20, KEEP, KEEP);
    wait_all_reach();
}

void hand_wave(int i) {
    float x_tmp, y_tmp, z_tmp;
    currentMode = "HAND WAVE";
    move_speed = 1.0f;
    if (is_stand()) {
        body_right(1);
        x_tmp = site_now[2][0];
        y_tmp = site_now[2][1];
        z_tmp = site_now[2][2];
        move_speed = 20.0f;
        for (int j = 0; j < i && !stopRequested; j++) {
            set_site(2, turn_x1, turn_y1, 50);
            wait_all_reach();
            set_site(2, turn_x1, turn_y1, -10);
            wait_all_reach();
        }
        move_speed = 1.0f;
        set_site(2, x_tmp, y_tmp, z_tmp);
        wait_all_reach();
        body_left(1);
    }
    if (!stopRequested) currentMode = "STAND";
}

void hand_shake(int i) {
    float x_tmp, y_tmp, z_tmp;
    currentMode = "HAND SHAKE";
    move_speed = 1.0f;
    if (is_stand()) {
        body_right(1);
        x_tmp = site_now[2][0];
        y_tmp = site_now[2][1];
        z_tmp = site_now[2][2];
        move_speed = 10.0f;
        for (int j = 0; j < i && !stopRequested; j++) {
            set_site(2, x_default - 30, y_start + 2 * y_step, 55);
            wait_all_reach();
            set_site(2, x_default - 30, y_start + 2 * y_step, 10);
            wait_all_reach();
        }
        move_speed = 1.0f;
        set_site(2, x_tmp, y_tmp, z_tmp);
        wait_all_reach();
        body_left(1);
    }
    if (!stopRequested) currentMode = "STAND";
}

// Action Dispatcher
struct ActionRequest {
    char action[20];
    int steps;
    float speed;
};

QueueHandle_t actionQueue = NULL;

void actionTask(void* parameter) {
    ActionRequest req;
    for (;;) {
        if (xQueueReceive(actionQueue, &req, portMAX_DELAY) == pdTRUE) {
            String act = String(req.action);
            if (act == "stop") {
                stopRequested = true;
                currentMode = "STOPPED";
                continue;
            }

            // Enable kinematics solver during active locomotion / gestures
            kinematicsActive = true;
            isActionRunning = true;
            stopRequested = false;
            speed_multiple = req.speed > 0 ? req.speed : 1.0f;

            Serial.printf("[ACTION] '%s' (%d steps, %.1fx)\n", act.c_str(), req.steps, speed_multiple);

            if (act == "forward") {
                if (!is_stand()) stand();
                step_forward(req.steps);
            } else if (act == "backward" || act == "back") {
                if (!is_stand()) stand();
                step_back(req.steps);
            } else if (act == "turn_left" || act == "left") {
                if (!is_stand()) stand();
                turn_left(req.steps);
            } else if (act == "turn_right" || act == "right") {
                if (!is_stand()) stand();
                turn_right(req.steps);
            } else if (act == "turn_180_left" || act == "turn_left_180" || act == "u_turn_left") {
                if (!is_stand()) stand();
                currentMode = "TURN 180° LEFT";
                turn_left(8);
            } else if (act == "turn_180_right" || act == "turn_right_180" || act == "u_turn_right") {
                if (!is_stand()) stand();
                currentMode = "TURN 180° RIGHT";
                turn_right(8);
            } else if (act == "stand_high" || act == "stand_tall" || act == "tall" || act == "high") {
                stand_high(z_high);
            } else if (act == "stand") {
                stand();
            } else if (act == "sit") {
                sit();
            } else if (act == "set_height_low" || act == "height_low" || act == "low" || act == "crouch") {
                setWalkingHeight("low");
            } else if (act == "set_height_normal" || act == "height_normal" || act == "normal") {
                setWalkingHeight("normal");
            } else if (act == "set_height_high" || act == "height_high" || act == "high_walk") {
                setWalkingHeight("high");
            } else if (act == "hand_shake" || act == "shake") {
                if (!is_stand()) stand();
                hand_shake(req.steps);
            } else if (act == "hand_wave" || act == "wave") {
                if (!is_stand()) stand();
                hand_wave(req.steps);
            }

            isActionRunning = false;
        }
    }
}

void executeAction(const String& act, int steps, float spd) {
    if (act == "stop") {
        stopRequested = true;
        currentMode = "STOPPED";
        return;
    }
    ActionRequest req;
    strncpy(req.action, act.c_str(), sizeof(req.action) - 1);
    req.action[sizeof(req.action) - 1] = '\0';
    req.steps = (steps > 0) ? steps : 1;
    req.speed = (spd > 0) ? spd : 1.0f;
    xQueueSend(actionQueue, &req, 0);
}

// ==========================================
// Serial Command Interface (Nano Bluetooth backwards compatibility)
// ==========================================
// w 0 1: stand | w 0 0: sit | w 1 x: fwd | w 2 x: back | w 3 x: right | w 4 x: left | w 5 x: shake | w 6 x: wave
// ==========================================
// Serial Telemetry & Command Interface
// ==========================================
bool serialTelemetryStream = true;

void sendSerialTelemetryJson() {
    String s = "{\"t\":";
    s += String(millis());
    s += ",\"robot\":\"" ROBOT_NAME "\"";
    s += ",\"version\":\"" ROBOT_VERSION "\"";
    s += ",\"pca\":"; s += (pcaReady ? "true" : "false");
    s += ",\"mode\":\""; s += (pcaReady ? currentMode : "PCA OFFLINE"); s += "\"";
    s += ",\"walk_height\":\""; s += currentWalkHeight; s += "\"";
    s += ",\"moving\":"; s += ((pcaReady && isActionRunning) ? "true" : "false");
    s += ",\"sites\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        s += "[";
        s += String(site_now[l][0], 1) + ",";
        s += String(site_now[l][1], 1) + ",";
        s += String(site_now[l][2], 1);
        s += "]";
        if (l < NUM_LEGS - 1) s += ",";
    }
    s += "],\"angles\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        s += "[";
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            s += String(currentAngles[l][j]);
            if (j < SERVOS_PER_LEG - 1) s += ",";
        }
        s += "]";
        if (l < NUM_LEGS - 1) s += ",";
    }
    s += "],\"pwr\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        s += "[";
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            s += (servoEnabled[l][j] ? "true" : "false");
            if (j < SERVOS_PER_LEG - 1) s += ",";
        }
        s += "]";
        if (l < NUM_LEGS - 1) s += ",";
    }
    s += "]}";
    Serial.println(s);
}

void parseSerialCommand(const String& cmd) {
    String trimmed = cmd;
    trimmed.trim();
    if (trimmed.length() == 0) return;

    // 1. Structured JSON Command from Companion GUI
    if (trimmed.startsWith("{") && trimmed.endsWith("}")) {
        // Action command: {"action":"forward","steps":3,"speed":1.0}
        int actIdx = trimmed.indexOf("\"action\"");
        if (actIdx >= 0) {
            int colon = trimmed.indexOf(':', actIdx);
            int q1 = trimmed.indexOf('"', colon);
            int q2 = trimmed.indexOf('"', q1 + 1);
            if (q1 >= 0 && q2 > q1) {
                String act = trimmed.substring(q1 + 1, q2);
                int steps = 1;
                float spd = 1.0f;
                int stIdx = trimmed.indexOf("\"steps\"");
                if (stIdx >= 0) {
                    int c = trimmed.indexOf(':', stIdx);
                    steps = trimmed.substring(c + 1).toInt();
                }
                int spIdx = trimmed.indexOf("\"speed\"");
                if (spIdx >= 0) {
                    int c = trimmed.indexOf(':', spIdx);
                    spd = trimmed.substring(c + 1).toFloat();
                }
                executeAction(act, steps, spd);
                return;
            }
        }

        // Walking Height command: {"height":"low"|"normal"|"high"}
        int hgtIdx = trimmed.indexOf("\"height\"");
        if (hgtIdx >= 0) {
            int colon = trimmed.indexOf(':', hgtIdx);
            int q1 = trimmed.indexOf('"', colon);
            int q2 = trimmed.indexOf('"', q1 + 1);
            if (q1 >= 0 && q2 > q1) {
                String hgt = trimmed.substring(q1 + 1, q2);
                setWalkingHeight(hgt);
                return;
            }
        }

        // Servo set: {"servo":{"leg":0,"joint":1,"angle":90}}
        int srvIdx = trimmed.indexOf("\"servo\"");
        if (srvIdx >= 0) {
            int lIdx = trimmed.indexOf("\"leg\"", srvIdx);
            int jIdx = trimmed.indexOf("\"joint\"", srvIdx);
            int aIdx = trimmed.indexOf("\"angle\"", srvIdx);
            if (lIdx >= 0 && jIdx >= 0 && aIdx >= 0) {
                int leg = trimmed.substring(trimmed.indexOf(':', lIdx) + 1).toInt();
                int joint = trimmed.substring(trimmed.indexOf(':', jIdx) + 1).toInt();
                int angle = trimmed.substring(trimmed.indexOf(':', aIdx) + 1).toInt();
                if (leg >= 0 && leg < NUM_LEGS && joint >= 0 && joint < SERVOS_PER_LEG) {
                    kinematicsActive = false;
                    currentAngles[leg][joint] = constrain(angle, 0, 180);
                    servoEnabled[leg][joint] = true;
                    setServoAngle(SERVO_CHANNELS[leg][joint], currentAngles[leg][joint]);
                }
                return;
            }
        }

        // Power set: {"power":{"target":"all","state":1}}
        int pwrIdx = trimmed.indexOf("\"power\"");
        if (pwrIdx >= 0) {
            int stIdx = trimmed.indexOf("\"state\"", pwrIdx);
            int state = (stIdx >= 0) ? trimmed.substring(trimmed.indexOf(':', stIdx) + 1).toInt() : 1;
            setMasterPower(state == 1);
            return;
        }

        // Center / Init: {"init":"all"}
        if (trimmed.indexOf("\"init\"") >= 0) {
            calibrateAllServos(trimmed.indexOf("\"wave\"") >= 0);
            return;
        }

        // Stream toggle: {"stream":true/false}
        int strIdx = trimmed.indexOf("\"stream\"");
        if (strIdx >= 0) {
            serialTelemetryStream = (trimmed.indexOf("true", strIdx) >= 0 || trimmed.indexOf("1", strIdx) >= 0);
            return;
        }

        // Telemetry request
        if (trimmed.indexOf("\"telemetry\"") >= 0 || trimmed.indexOf("\"status\"") >= 0) {
            sendSerialTelemetryJson();
            return;
        }
    }

    // 2. Legacy Nano Bluetooth Protocol ("w <mode> <steps>")
    if (trimmed.startsWith("w ") || trimmed.startsWith("W ")) {
        int firstSpace = trimmed.indexOf(' ');
        int secondSpace = trimmed.indexOf(' ', firstSpace + 1);
        if (firstSpace > 0) {
            int mode = trimmed.substring(firstSpace + 1, secondSpace > 0 ? secondSpace : trimmed.length()).toInt();
            int steps = (secondSpace > 0) ? trimmed.substring(secondSpace + 1).toInt() : 1;

            switch (mode) {
                case 0:
                    if (steps) executeAction("stand", 1, 1.0f);
                    else executeAction("sit", 1, 1.0f);
                    break;
                case 1: executeAction("forward", steps, 1.0f); break;
                case 2: executeAction("backward", steps, 1.0f); break;
                case 3: executeAction("turn_right", steps, 1.0f); break;
                case 4: executeAction("turn_left", steps, 1.0f); break;
                case 5: executeAction("hand_shake", steps, 1.0f); break;
                case 6: executeAction("hand_wave", steps, 1.0f); break;
                default: Serial.println(F("[CMD] Unknown action code")); break;
            }
        }
    } else if (trimmed.equalsIgnoreCase("stand")) {
        executeAction("stand", 1, 1.0f);
    } else if (trimmed.equalsIgnoreCase("sit")) {
        executeAction("sit", 1, 1.0f);
    } else if (trimmed.equalsIgnoreCase("stop")) {
        executeAction("stop", 1, 1.0f);
    } else if (trimmed.equalsIgnoreCase("status") || trimmed.equalsIgnoreCase("telemetry")) {
        sendSerialTelemetryJson();
    }
}

// ==========================================
// OLED Display Helpers
// ==========================================
void drawSplashScreen() {
    if (!oledReady) return;
    display.clearDisplay();
    display.fillRect(0, 0, SCREEN_WIDTH, 14, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(12, 3);
    display.print(F("ARK-BOT ROBOT"));

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 18);
    display.println(F("Action Commander v1.0.1"));
    display.setCursor(4, 30);
    display.print(F("Ant: "));
    display.println(useExternalAntenna ? F("External (IPEX)") : F("Internal Ceramic"));
    
    display.drawRect(4, 48, 120, 10, SSD1306_WHITE);
    display.fillRect(6, 50, 60, 6, SSD1306_WHITE);
    display.display();
}

void drawMainDashboard(bool heartbeat, int cycleIndex) {
    if (!oledReady) return;
    display.clearDisplay();

    // Top Header
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F("ARK-BOT v1.0.1"));
    if (heartbeat) display.fillCircle(123, 3, 2, SSD1306_WHITE);
    else display.drawCircle(123, 3, 2, SSD1306_WHITE);
    display.drawFastHLine(0, 9, SCREEN_WIDTH, SSD1306_WHITE);

    // Matrix display
    const int colX[2] = {0, 66};
    const int rowY[2] = {12, 32};
    for (int l = 0; l < NUM_LEGS; l++) {
        int x = colX[l % 2];
        int y = rowY[l / 2];
        display.setCursor(x, y);
        display.print(LEG_NAMES[l]); display.print(F(":"));
        display.setCursor(x, y + 9);
        display.print(currentAngles[l][0]); display.print(F("/"));
        display.print(currentAngles[l][1]); display.print(F("/"));
        display.print(currentAngles[l][2]);
    }

    display.drawFastHLine(0, 52, SCREEN_WIDTH, SSD1306_WHITE);
    display.setCursor(0, 55);
    if (!pcaReady) {
        display.print(F("! PCA9685 OFFLINE !"));
    } else if (cycleIndex == 0) {
        display.print(F("IP: ")); display.print(activeIp);
    } else if (cycleIndex == 1) {
        display.print(F("MD: ")); display.print(currentMode);
    } else {
        display.print(useExternalAntenna ? F("ANT:EXT ") : F("ANT:INT "));
        if (wifiStaConnected) {
            display.print(WiFi.RSSI()); display.print(F("dBm"));
        } else {
            display.print(F("AP"));
        }
    }
    display.display();
}

// ==========================================
// Web Server API Handlers
// ==========================================
void handleCommander() {
    server.send_P(200, "text/html", COMMANDER_HTML);
}

void handleCalib() {
    server.send_P(200, "text/html", CALIB_HTML);
}

void handleSetup() {
    server.send_P(200, "text/html", SETUP_HTML);
}

void handleStatus() {
    // Dynamic PCA9685 connection re-probe if state changes
    if (!pcaReady && probeI2C(PCA9685_I2C_ADDRESS)) {
        pwm.begin();
        pwm.setOscillatorFrequency(27000000);
        pwm.setPWMFreq(SERVO_FREQ);
        pcaReady = true;
        currentMode = "ALL @ 90deg";
        Serial.println(F("[OK] PCA9685 Driver connected (0x40)"));
    } else if (pcaReady && !probeI2C(PCA9685_I2C_ADDRESS)) {
        pcaReady = false;
        currentMode = "PCA OFFLINE";
        Serial.println(F("[WARN] PCA9685 disconnected (0x40)"));
    }

    static int cachedRssi = -50;
    static unsigned long lastRssiCheck = 0;
    if (millis() - lastRssiCheck >= 2000) {
        lastRssiCheck = millis();
        cachedRssi = (WiFi.status() == WL_CONNECTED) ? WiFi.RSSI() : 0;
    }

    String json = "{";
    json += "\"robot\":\"" + String(ROBOT_NAME) + "\",";
    json += "\"version\":\"" + String(ROBOT_VERSION) + "\",";
    json += "\"pca\":"; json += (pcaReady ? "true" : "false"); json += ",";
    json += "\"pcaReady\":"; json += (pcaReady ? "true" : "false"); json += ",";
    json += "\"mode\":\"" + (pcaReady ? currentMode : "PCA OFFLINE") + "\",";
    json += "\"walk_height\":\"" + currentWalkHeight + "\",";
    json += "\"moving\":" + String((pcaReady && isActionRunning) ? "true" : "false") + ",";
    json += "\"uptime\":" + String(millis() / 1000) + ",";
    json += "\"extAntenna\":" + String(useExternalAntenna ? "true" : "false") + ",";
    json += "\"rssi\":" + String(cachedRssi) + ",";
    
    // Angles matrix
    json += "\"angles\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        json += "[";
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            json += String(currentAngles[l][j]);
            if (j < SERVOS_PER_LEG - 1) json += ",";
        }
        json += "]";
        if (l < NUM_LEGS - 1) json += ",";
    }
    json += "],";

    // Real-time Cartesian site coordinates
    json += "\"sites\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        json += "[";
        json += String(site_now[l][0], 1) + ",";
        json += String(site_now[l][1], 1) + ",";
        json += String(site_now[l][2], 1);
        json += "]";
        if (l < NUM_LEGS - 1) json += ",";
    }
    json += "],";

    // Power matrix
    json += "\"pwr\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        json += "[";
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            json += String(servoEnabled[l][j] ? "true" : "false");
            if (j < SERVOS_PER_LEG - 1) json += ",";
        }
        json += "]";
        if (l < NUM_LEGS - 1) json += ",";
    }
    json += "],";
    json += "\"enabled\":[";
    for (int l = 0; l < NUM_LEGS; l++) {
        json += "[";
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            json += String(servoEnabled[l][j] ? "true" : "false");
            if (j < SERVOS_PER_LEG - 1) json += ",";
        }
        json += "]";
        if (l < NUM_LEGS - 1) json += ",";
    }
    json += "]}";

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Connection", "close");
    server.send(200, "application/json", json);
}

void handleAction() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (server.hasArg("height")) {
        setWalkingHeight(server.arg("height"));
    }

    if (!server.hasArg("action")) {
        server.send(200, "application/json", "{\"status\":\"ok\",\"walk_height\":\"" + currentWalkHeight + "\"}");
        return;
    }

    if (!pcaReady && probeI2C(PCA9685_I2C_ADDRESS)) {
        pwm.begin();
        pwm.setOscillatorFrequency(27000000);
        pwm.setPWMFreq(SERVO_FREQ);
        pcaReady = true;
    }

    if (!pcaReady) {
        server.send(503, "application/json", "{\"status\":\"error\",\"error\":\"PCA9685 driver offline (0x40)\"}");
        return;
    }

    String act = server.arg("action");
    int steps = server.hasArg("steps") ? server.arg("steps").toInt() : 1;
    float spd = server.hasArg("speed") ? server.arg("speed").toFloat() : 1.0f;

    executeAction(act, steps, spd);
    playStepChime();

    server.send(200, "application/json", "{\"status\":\"accepted\",\"action\":\"" + act + "\"}");
}

void handleSetAntenna() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (!server.hasArg("type")) {
        server.send(400, "application/json", "{\"error\":\"Missing 'type' parameter\"}");
        return;
    }

    String type = server.arg("type");
    bool isExt = (type.equalsIgnoreCase("external") || type == "1" || type.equalsIgnoreCase("ext"));
    setAntenna(isExt, true);
    playStepChime();

    String resp = "{\"success\":true,\"extAntenna\":" + String(isExt ? "true" : "false") + ",\"rssi\":" + String(WiFi.RSSI()) + "}";
    server.send(200, "application/json", resp);
}

void handleSetServo() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (!server.hasArg("leg") || !server.hasArg("joint") || !server.hasArg("angle")) {
        server.send(400, "application/json", "{\"error\":\"Missing parameters (leg, joint, angle)\"}");
        return;
    }

    int leg = server.arg("leg").toInt();
    int joint = server.arg("joint").toInt();
    int angle = server.arg("angle").toInt();

    if (leg < 0 || leg >= NUM_LEGS || joint < 0 || joint >= SERVOS_PER_LEG) {
        server.send(400, "application/json", "{\"error\":\"Invalid index\"}");
        return;
    }

    // Disable kinematics so manual slider is directly set
    kinematicsActive = false;
    currentAngles[leg][joint] = constrain(angle, 0, 180);
    servoEnabled[leg][joint] = true;
    setServoAngle(SERVO_CHANNELS[leg][joint], currentAngles[leg][joint]);

    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handlePower() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    String target = server.hasArg("target") ? server.arg("target") : "all";
    bool state = server.hasArg("state") ? (server.arg("state").toInt() == 1) : true;

    if (target == "all") {
        setMasterPower(state);
    } else if (target == "leg") {
        int leg = server.arg("leg").toInt();
        setLegPower(leg, state);
    } else if (target == "joint") {
        int leg = server.arg("leg").toInt();
        int joint = server.arg("joint").toInt();
        setServoPower(leg, joint, state);
    }
    handleStatus();
}

void handleInit() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    String type = server.hasArg("type") ? server.arg("type") : "all";
    calibrateAllServos(type == "wave");
    handleStatus();
}

void handleBeep() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    playStepChime();
    server.send(200, "application/json", "{\"status\":\"beeped\"}");
}

// Wi-Fi Helper Functions
String escapeJsonStr(const String& s) {
    String res = "";
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '"') res += "\\\"";
        else if (c == '\\') res += "\\\\";
        else if (c == '\b') res += "\\b";
        else if (c == '\f') res += "\\f";
        else if (c == '\n') res += "\\n";
        else if (c == '\r') res += "\\r";
        else if (c == '\t') res += "\\t";
        else res += c;
    }
    return res;
}

String getAuthModeString(wifi_auth_mode_t authMode) {
    switch (authMode) {
        case WIFI_AUTH_OPEN: return "Open";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        default: return "WPA2";
    }
}

void handleWifiScan() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    int n = WiFi.scanNetworks(false, false);
    String json = "[";
    for (int i = 0; i < n; ++i) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"ssid\":\"" + escapeJsonStr(WiFi.SSID(i)) + "\",";
        json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
        json += "\"channel\":" + String(WiFi.channel(i)) + ",";
        json += "\"auth\":\"" + getAuthModeString(WiFi.encryptionType(i)) + "\",";
        json += "\"connected\":" + String(WiFi.status() == WL_CONNECTED && WiFi.SSID(i) == WiFi.SSID() ? "true" : "false");
        json += "}";
    }
    json += "]";
    WiFi.scanDelete();
    server.send(200, "application/json", json);
}

void handleWifiStatus() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    bool isConnected = (WiFi.status() == WL_CONNECTED);
    String json = "{";
    json += "\"connected\":" + String(isConnected ? "true" : "false") + ",";
    json += "\"staSsid\":\"" + escapeJsonStr(isConnected ? WiFi.SSID() : staSsid) + "\",";
    json += "\"staIp\":\"" + (isConnected ? WiFi.localIP().toString() : "") + "\",";
    json += "\"apSsid\":\"" + String(WIFI_AP_SSID) + "\",";
    json += "\"apIp\":\"" + WiFi.softAPIP().toString() + "\",";
    json += "\"mac\":\"" + WiFi.macAddress() + "\",";
    json += "\"hostname\":\"" + String(MDNS_HOSTNAME) + ".local\",";
    json += "\"rssi\":" + String(isConnected ? WiFi.RSSI() : 0) + ",";
    json += "\"extAntenna\":" + String(useExternalAntenna ? "true" : "false") + ",";
    json += "\"uptime\":" + String(millis() / 1000);
    json += "}";
    server.send(200, "application/json", json);
}

void handleWifiSave() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (!server.hasArg("ssid")) {
        server.send(400, "application/json", "{\"error\":\"Missing 'ssid' parameter\"}");
        return;
    }
    staSsid = server.arg("ssid");
    staPass = server.hasArg("pass") ? server.arg("pass") : "";
    
    preferences.begin("ark_wifi", false);
    preferences.putString("ssid", staSsid);
    preferences.putString("pass", staPass);
    preferences.end();

    server.send(200, "application/json", "{\"success\":true,\"message\":\"Credentials saved. Connecting...\"}");
    WiFi.disconnect();
    WiFi.begin(staSsid.c_str(), staPass.c_str());
}

void handleWifiReconnect() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Reconnecting...\"}");
    WiFi.disconnect();
    WiFi.begin(staSsid.c_str(), staPass.c_str());
}

void handleWifiReset() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    preferences.begin("ark_wifi", false);
    preferences.clear();
    preferences.end();
    staSsid = WIFI_STA_SSID;
    staPass = WIFI_STA_PASS;
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Reset to defaults\"}");
}

void handleReboot() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Rebooting...\"}");
    delay(400);
    ESP.restart();
}

// ==========================================
// Setup & Loop
// ==========================================
void setup() {
    Serial.begin(115200);
    Serial.setTimeout(5);

    initBuzzer();
    initAntenna();
    initKinematicsSites();

    // Preferences: Wi-Fi
    preferences.begin("ark_wifi", false);
    staSsid = preferences.getString("ssid", WIFI_STA_SSID);
    staPass = preferences.getString("pass", WIFI_STA_PASS);
    preferences.end();

    delay(300);
    Serial.println(F("\n=========================================="));
    Serial.println(F("      ARK-BOT CYBER MOTION COMMANDER      "));
    Serial.println(F("         Version 1.0.0 (ESP32-C6)         "));
    Serial.println(F("=========================================="));

    // Wi-Fi Setup
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_CHANNEL, 0, WIFI_MAX_CONN);
    WiFi.begin(staSsid.c_str(), staPass.c_str());

    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
        delay(250);
        Serial.print(F("."));
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        wifiStaConnected = true;
        activeIp = WiFi.localIP().toString();
        Serial.printf("[OK] Connected to %s! IP: %s\n", staSsid.c_str(), activeIp.c_str());
    } else {
        wifiStaConnected = false;
        activeIp = WiFi.softAPIP().toString();
        Serial.println(F("[WARN] Home Wi-Fi offline. Using SoftAP mode."));
    }

    if (MDNS.begin(MDNS_HOSTNAME)) {
        MDNS.addService("http", "tcp", 80);
        Serial.printf("[OK] mDNS: http://%s.local\n", MDNS_HOSTNAME);
    }

    // Web Routes
    server.on("/", HTTP_GET, handleCommander);
    server.on("/calib", HTTP_GET, handleCalib);
    server.on("/setup", HTTP_GET, handleSetup);
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/action", HTTP_POST, handleAction);
    server.on("/api/wifi/scan", HTTP_GET, handleWifiScan);
    server.on("/api/wifi/status", HTTP_GET, handleWifiStatus);
    server.on("/api/wifi/save", HTTP_POST, handleWifiSave);
    server.on("/api/wifi/reconnect", HTTP_POST, handleWifiReconnect);
    server.on("/api/wifi/reset", HTTP_POST, handleWifiReset);
    server.on("/api/reboot", HTTP_POST, handleReboot);
    server.on("/api/antenna", HTTP_POST, handleSetAntenna);
    server.on("/api/servo", HTTP_POST, handleSetServo);
    server.on("/api/power", HTTP_POST, handlePower);
    server.on("/api/init", HTTP_POST, handleInit);
    server.on("/api/beep", HTTP_POST, handleBeep);
    server.begin();
    Serial.println(F("[OK] Web Server started on port 80"));

    // I2C Bus & OLED
    Wire.begin();
    if (display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        oledReady = true;
        drawSplashScreen();
    }

    playBootChime();

    // PCA9685
    if (probeI2C(PCA9685_I2C_ADDRESS)) {
        pwm.begin();
        pwm.setOscillatorFrequency(27000000);
        pwm.setPWMFreq(SERVO_FREQ);
        pcaReady = true;
        currentMode = "ALL @ 90deg";
        Serial.println(F("[OK] PCA9685 Driver initialized (0x40)"));
        delay(300);
        // Start calibrated to 90 degrees neutral position
        calibrateAllServos(true);
    } else {
        pcaReady = false;
        currentMode = "PCA OFFLINE";
        Serial.println(F("[FAIL] PCA9685 NOT found (0x40)!"));
        playErrorChime();
    }

    // Create FreeRTOS Tasks
    actionQueue = xQueueCreate(8, sizeof(ActionRequest));
    
    xTaskCreatePinnedToCore(
        kinematicsTask,
        "KinematicsTask",
        4096,
        NULL,
        2, // High priority
        &kinematicsTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        actionTask,
        "ActionTask",
        4096,
        NULL,
        1, // Normal priority
        &actionTaskHandle,
        0
    );

    Serial.println(F("[OK] ARK-BOT System Ready in Neutral 90 deg Calibrated Stance."));
}

void loop() {
    server.handleClient();

    // Non-blocking High-Speed Serial Command Parsing
    static String serialCmdBuf = "";
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialCmdBuf.length() > 0) {
                parseSerialCommand(serialCmdBuf);
                serialCmdBuf = "";
            }
        } else {
            if (serialCmdBuf.length() < 256) {
                serialCmdBuf += c;
            }
        }
    }

    // Wi-Fi State monitor (throttled to 1s to prevent TCP/IP driver stalls)
    static unsigned long lastWifiCheck = 0;
    if (millis() - lastWifiCheck >= 1000) {
        lastWifiCheck = millis();
        bool isConnected = (WiFi.status() == WL_CONNECTED);
        if (isConnected != wifiStaConnected) {
            wifiStaConnected = isConnected;
            activeIp = isConnected ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
        }
    }

    // Periodic Serial JSON Telemetry Stream (20Hz / 50ms)
    static unsigned long lastSerialTelem = 0;
    if (serialTelemetryStream && (millis() - lastSerialTelem >= 50)) {
        lastSerialTelem = millis();
        sendSerialTelemetryJson();
    }

    static unsigned long lastOled = 0;
    static bool blink = false;
    static int cycle = 0;

    if (millis() - lastOled >= 500) {
        lastOled = millis();
        blink = !blink;
        cycle = (cycle + 1) % 9;
        drawMainDashboard(blink, cycle / 3);
    }
}
