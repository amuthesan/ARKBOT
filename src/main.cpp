#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESPmDNS.h>

#include "config.h"
#include "web_page.h"

// Hardware Instances
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_I2C_ADDRESS);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);

// State tracking
int currentAngles[NUM_LEGS][SERVOS_PER_LEG];
bool servoEnabled[NUM_LEGS][SERVOS_PER_LEG];
bool oledReady = false;
bool pcaReady = false;
bool wifiStaConnected = false;
String activeIp = "192.168.4.1";
String currentMode = "ALL @ 90deg";

// Forward declarations
void playStepChime();
void playReadyChime();
void playBootChime();
void playErrorChime();
void setLegJointAngle(int leg, int joint, int angle);
void setServoPower(int leg, int joint, bool enable);
void setMasterPower(bool enable);
void setLegPower(int leg, bool enable);
void calibrateAllServos(bool sequential);

// ==========================================
// Buzzer Audio Feedback Helpers (LEDC PWM)
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
    playTone(1047, 70);  // C6
    delay(30);
    playTone(1319, 70);  // E6
    delay(30);
    playTone(1568, 70);  // G6
    delay(30);
    playTone(2093, 120); // C7
}

void playStepChime() {
    playTone(1760, 25);  // A6
}

void playReadyChime() {
    playTone(1319, 80);  // E6
    delay(40);
    playTone(1760, 160); // A6
}

void playErrorChime() {
    playTone(440, 150);  // A4
    delay(60);
    playTone(349, 250);  // F4
}

// ==========================================
// I2C Helper
// ==========================================
bool probeI2C(uint8_t address) {
    Wire.beginTransmission(address);
    return (Wire.endTransmission() == 0);
}

// ==========================================
// Servo Control via PCA9685
// ==========================================
int angleToPulse(int angle) {
    angle = constrain(angle, 0, 180);
    return map(angle, 0, 180, SERVOMIN_COUNT, SERVOMAX_COUNT);
}

void setServoAngle(int channel, int angle) {
    if (!pcaReady || channel < 0 || channel > 15) return;
    int pulse = angleToPulse(angle);
    pwm.setPWM(channel, 0, pulse);
}

void setServoPower(int leg, int joint, bool enable) {
    if (leg < 0 || leg >= NUM_LEGS || joint < 0 || joint >= SERVOS_PER_LEG) return;
    int ch = SERVO_CHANNELS[leg][joint];
    servoEnabled[leg][joint] = enable;
    if (enable) {
        setServoAngle(ch, currentAngles[leg][joint]);
    } else {
        if (pcaReady) {
            pwm.setPWM(ch, 0, 0); // Release motor (0 duty cycle stops PWM pulse)
        }
    }
}

void setMasterPower(bool enable) {
    for (int l = 0; l < NUM_LEGS; l++) {
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            setServoPower(l, j, enable);
        }
    }
    currentMode = enable ? "ALL ENERGIZED" : "ALL RELEASED";
    playStepChime();
}

void setLegPower(int leg, bool enable) {
    if (leg < 0 || leg >= NUM_LEGS) return;
    for (int j = 0; j < SERVOS_PER_LEG; j++) {
        setServoPower(leg, j, enable);
    }
    currentMode = String(LEG_NAMES[leg]) + (enable ? " ON" : " OFF");
    playStepChime();
}

void setLegJointAngle(int leg, int joint, int angle) {
    if (leg >= 0 && leg < NUM_LEGS && joint >= 0 && joint < SERVOS_PER_LEG) {
        angle = constrain(angle, 0, 180);
        int ch = SERVO_CHANNELS[leg][joint];
        currentAngles[leg][joint] = angle;
        servoEnabled[leg][joint] = true;
        setServoAngle(ch, angle);
    }
}

// ==========================================
// OLED Display Helpers
// ==========================================
void drawSplashScreen() {
    if (!oledReady) return;
    display.clearDisplay();
    
    // Header Banner
    display.fillRect(0, 0, SCREEN_WIDTH, 14, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(18, 3);
    display.print(F("ARK-BOT ROBOT"));

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 20);
    display.println(F("XIAO ESP32-C6 (WiFi 6)"));
    display.setCursor(4, 32);
    display.println(F("Visual Calibrator"));
    
    display.drawRect(4, 48, 120, 10, SSD1306_WHITE);
    display.fillRect(6, 50, 60, 6, SSD1306_WHITE);
    display.display();
}

void drawPcaMissingScreen(bool blinkState) {
    if (!oledReady) return;
    display.clearDisplay();

    // Inverted Header Banner
    display.fillRect(0, 0, SCREEN_WIDTH, 13, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(6, 3);
    display.println(F("! HARDWARE WARNING !"));

    // Status
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 16);
    display.print(F("PCA9685: "));
    if (blinkState) {
        display.println(F("[MISSING]"));
    } else {
        display.println(F("[OFFLINE]"));
    }

    display.setCursor(0, 27);
    display.println(F("I2C Addr 0x40 offline"));

    display.drawFastHLine(0, 39, SCREEN_WIDTH, SSD1306_WHITE);

    // Help Text
    display.setCursor(0, 43);
    display.println(F("Check SDA:D4 / SCL:D5"));
    display.setCursor(0, 54);
    display.println(F("Connect PCA driver"));

    display.display();
}

void updateCalibrationProgress(int leg, int joint, int progressPercent) {
    if (!oledReady) return;
    display.clearDisplay();

    // Title
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(8, 2);
    display.println(F("ARK-BOT CALIBRATE"));
    display.drawFastHLine(0, 12, SCREEN_WIDTH, SSD1306_WHITE);

    // Current Target
    display.setCursor(0, 18);
    display.print(F("Leg: "));
    display.print(LEG_NAMES[leg]);
    display.print(F(" ("));
    display.print(LEG_FULL_NAMES[leg]);
    display.println(F(")"));

    display.setCursor(0, 30);
    display.print(F("Joint: "));
    display.print(JOINT_NAMES[joint]);
    display.print(F(" [Ch "));
    display.print(SERVO_CHANNELS[leg][joint]);
    display.println(F("]"));

    display.setCursor(0, 42);
    display.print(F("Angle: 90 deg (CENTER)"));

    // Progress Bar
    display.drawRect(0, 54, 128, 8, SSD1306_WHITE);
    int barWidth = map(progressPercent, 0, 100, 0, 124);
    display.fillRect(2, 56, barWidth, 4, SSD1306_WHITE);

    display.display();
}

void drawMainDashboard(bool heartbeat, bool showIpCycle) {
    if (!oledReady) return;
    display.clearDisplay();

    // Top status line
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F("ARK-BOT "));
    
    // Heartbeat indicator
    if (heartbeat) {
        display.fillCircle(123, 3, 2, SSD1306_WHITE);
    } else {
        display.drawCircle(123, 3, 2, SSD1306_WHITE);
    }
    display.drawFastHLine(0, 9, SCREEN_WIDTH, SSD1306_WHITE);

    // Grid for 4 legs (FR, FL, RR, RL)
    const int colX[2] = {0, 66};
    const int rowY[2] = {12, 32};

    for (int l = 0; l < NUM_LEGS; l++) {
        int col = l % 2;
        int row = l / 2;
        int x = colX[col];
        int y = rowY[row];

        display.setCursor(x, y);
        display.print(LEG_NAMES[l]);
        display.print(F(":"));

        display.setCursor(x, y + 9);
        display.print(currentAngles[l][0]);
        display.print(F("/"));
        display.print(currentAngles[l][1]);
        display.print(F("/"));
        display.print(currentAngles[l][2]);
    }

    display.drawFastHLine(0, 52, SCREEN_WIDTH, SSD1306_WHITE);
    display.setCursor(0, 55);
    
    // Bottom banner cycles between Mode and WebUI IP address
    if (showIpCycle) {
        display.print(F("IP: "));
        display.print(activeIp);
    } else {
        display.print(F("MD: "));
        display.print(currentMode);
    }

    display.display();
}

// ==========================================
// Initialization & Calibration Routines
// ==========================================
bool initPCA9685() {
    if (!probeI2C(PCA9685_I2C_ADDRESS)) {
        return false;
    }
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);  // 27MHz internal oscillator
    pwm.setPWMFreq(SERVO_FREQ);            // 50Hz for standard servos
    return true;
}

void calibrateAllServos(bool sequential) {
    Serial.println(F("\n[ARK-BOT] Initializing / Centering all 12 leg joints to 90 degrees..."));

    int totalServos = NUM_LEGS * SERVOS_PER_LEG;
    int count = 0;

    for (int i = 0; i < NUM_LEGS; i++) {
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            count++;
            int progress = (count * 100) / totalServos;

            setLegJointAngle(i, j, DEFAULT_CALIBRATION_ANGLE);
            servoEnabled[i][j] = true;

            if (sequential) {
                updateCalibrationProgress(i, j, progress);
                playStepChime();
                delay(90);
            }
        }
    }

    currentMode = "ALL @ 90deg";
    playReadyChime();
    Serial.println(F("[SUCCESS] All 12 servos calibrated at 90 deg neutral position."));
    drawMainDashboard(true, false);
}

// ==========================================
// Web Server API Handlers
// ==========================================
void handleRoot() {
    server.send_P(200, "text/html", INDEX_HTML);
}

void handleStatus() {
    String json = "{";
    json += "\"robot\":\"" + String(ROBOT_NAME) + "\",";
    json += "\"pcaReady\":" + String(pcaReady ? "true" : "false") + ",";
    json += "\"mode\":\"" + currentMode + "\",";
    json += "\"uptime\":" + String(millis() / 1000) + ",";
    
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

    // Power enabled matrix
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
    server.send(200, "application/json", json);
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
        server.send(400, "application/json", "{\"error\":\"Invalid leg or joint index\"}");
        return;
    }

    setLegJointAngle(leg, joint, angle);
    currentMode = "MANUAL CONTROL";

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
        currentMode = String(LEG_NAMES[leg]) + " J" + String(joint) + (state ? " ON" : " OFF");
    }

    handleStatus();
}

void handleInit() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    String type = server.hasArg("type") ? server.arg("type") : "all";

    if (type == "wave") {
        calibrateAllServos(true);
    } else {
        calibrateAllServos(false);
    }

    handleStatus();
}

void handleBeep() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    playStepChime();
    server.send(200, "application/json", "{\"status\":\"beeped\"}");
}

// ==========================================
// Setup & Loop
// ==========================================
void setup() {
    Serial.begin(115200);

    // Initialize all servo states
    for (int l = 0; l < NUM_LEGS; l++) {
        for (int j = 0; j < SERVOS_PER_LEG; j++) {
            currentAngles[l][j] = DEFAULT_CALIBRATION_ANGLE;
            servoEnabled[l][j] = true;
        }
    }

    // Initialize buzzer with ESP32 LEDC PWM
    initBuzzer();

    delay(500);
    Serial.println(F("\n=========================================="));
    Serial.println(F("         ARK-BOT QUADRUPED SYSTEM         "));
    Serial.println(F("    Visual Calibrator (ESP32-C6 + PCA)    "));
    Serial.println(F("=========================================="));

    // 1. Initialize Wi-Fi (Connect to Home WiFi + SoftAP Fallback)
    WiFi.mode(WIFI_AP_STA);
    
    // Start SoftAP as fallback
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_CHANNEL, 0, WIFI_MAX_CONN);

    // Connect to Home Wi-Fi
    Serial.printf("[WiFi] Connecting to %s ...\n", WIFI_STA_SSID);
    WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASS);

    // Wait up to 6 seconds for Wi-Fi connection
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 6000) {
        delay(250);
        Serial.print(F("."));
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        wifiStaConnected = true;
        activeIp = WiFi.localIP().toString();
        Serial.printf("[OK] Connected to %s!\n", WIFI_STA_SSID);
        Serial.print(F("[OK] Home Web UI URL: http://"));
        Serial.println(activeIp);
    } else {
        wifiStaConnected = false;
        activeIp = WiFi.softAPIP().toString();
        Serial.println(F("[WARN] Could not connect to Home Wi-Fi. Using SoftAP mode."));
        Serial.print(F("[OK] SoftAP URL: http://"));
        Serial.println(activeIp);
    }

    // Initialize mDNS
    if (MDNS.begin(MDNS_HOSTNAME)) {
        MDNS.addService("http", "tcp", 80);
        Serial.printf("[OK] mDNS responder started: http://%s.local\n", MDNS_HOSTNAME);
    }

    // 2. Configure Web Server Routes
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/servo", HTTP_POST, handleSetServo);
    server.on("/api/power", HTTP_POST, handlePower);
    server.on("/api/init", HTTP_POST, handleInit);
    server.on("/api/beep", HTTP_POST, handleBeep);
    server.begin();
    Serial.println(F("[OK] HTTP Web Server started on port 80"));

    // 3. Initialize I2C Bus (SDA: D4/GPIO22, SCL: D5/GPIO23)
    Wire.begin();

    // 4. Initialize OLED Display
    if (display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        oledReady = true;
        Serial.println(F("[OK] SSD1306 OLED initialized (0x3C)"));
        drawSplashScreen();
    } else {
        Serial.println(F("[FAIL] SSD1306 OLED not found at 0x3C!"));
    }

    // Play boot melody
    playBootChime();

    // 5. Probe and Initialize PCA9685
    if (initPCA9685()) {
        pcaReady = true;
        Serial.println(F("[OK] PCA9685 Servo Driver initialized (0x40) at 50Hz"));
        delay(600);
        calibrateAllServos(true);
    } else {
        pcaReady = false;
        Serial.println(F("[FAIL] PCA9685 NOT found at I2C address 0x40!"));
        playErrorChime();
        drawPcaMissingScreen(true);
    }
}

void loop() {
    // Handle incoming Web UI client requests
    server.handleClient();

    static unsigned long lastUpdate = 0;
    static bool blinkState = false;
    static int cycleCounter = 0;

    // Refresh every 500ms
    if (millis() - lastUpdate >= 500) {
        lastUpdate = millis();
        blinkState = !blinkState;
        cycleCounter = (cycleCounter + 1) % 6; // Toggle IP vs Mode every 3 seconds

        if (pcaReady) {
            // Verify PCA9685 is still connected
            if (!probeI2C(PCA9685_I2C_ADDRESS)) {
                pcaReady = false;
                Serial.println(F("[WARN] PCA9685 disconnected from I2C bus!"));
                playErrorChime();
                drawPcaMissingScreen(true);
            } else {
                drawMainDashboard(blinkState, cycleCounter >= 3);
            }
        } else {
            // PCA not ready: probe I2C 0x40 for hot-plug reconnection
            if (probeI2C(PCA9685_I2C_ADDRESS)) {
                Serial.println(F("[OK] PCA9685 detected! Initializing..."));
                if (initPCA9685()) {
                    pcaReady = true;
                    calibrateAllServos(true);
                    drawMainDashboard(true, false);
                }
            } else {
                drawPcaMissingScreen(blinkState);
            }
        }
    }
}
