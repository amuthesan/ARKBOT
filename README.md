# ARK-BOT Quadruped Robot Controller

High-performance quadruped robot controller firmware built for the **Seeed Studio XIAO ESP32-C6** with **PCA9685 16-Channel I2C PWM Servo Driver**, **0.96" SSD1306 OLED Display**, **LEDC Buzzer**, and an embedded **Wi-Fi 6 Web UI**.

> **Migration Note**: This codebase modernizes the original Arduino Nano (Regis Hsu) design. Instead of driving servos directly from GPIO pins D2–D13 with `Servo.h`, all 12 servos now connect to the dedicated **PCA9685 I2C driver** (powered at 5V/6V), controlled by the **XIAO ESP32-C6**.

---

## PCA9685 16-Channel Servo Wiring Guide

To make upgrading from the Arduino Nano seamless, each servo connects to the **PCA9685 port number matching its original Arduino Nano pin (D2 $\to$ Port 2, D3 $\to$ Port 3, etc.)**:

### Port-by-Port Wiring Table

| PCA9685 Port | Leg | Joint | Former Nano Pin |
| :---: | :--- | :--- | :---: |
| **0** | *(Unused)* | — | — |
| **1** | *(Unused)* | — | — |
| **2** | **Front-Right (FR)** | **Femur (Thigh)** | `D2` |
| **3** | **Front-Right (FR)** | **Tibia (Calf)** | `D3` |
| **4** | **Front-Right (FR)** | **Coxa (Hip)** | `D4` |
| **5** | **Rear-Right (RR)** | **Femur (Thigh)** | `D5` |
| **6** | **Rear-Right (RR)** | **Tibia (Calf)** | `D6` |
| **7** | **Rear-Right (RR)** | **Coxa (Hip)** | `D7` |
| **8** | **Front-Left (FL)** | **Femur (Thigh)** | `D8` |
| **9** | **Front-Left (FL)** | **Tibia (Calf)** | `D9` |
| **10** | **Front-Left (FL)** | **Coxa (Hip)** | `D10` |
| **11** | **Rear-Left (RL)** | **Femur (Thigh)** | `D11` |
| **12** | **Rear-Left (RL)** | **Coxa (Hip)** | `D12` |
| **13** | **Rear-Left (RL)** | **Tibia (Calf)** | `D13` |
| **14** | *(Unused)* | — | — |
| **15** | *(Unused)* | — | — |

---

### Leg & Joint Mapping Summary

| Leg | Coxa (Hip) | Femur (Thigh) | Tibia (Calf) |
| :--- | :---: | :---: | :---: |
| **Front-Right (FR)** | **Port 4** *(D4)* | **Port 2** *(D2)* | **Port 3** *(D3)* |
| **Front-Left (FL)** | **Port 10** *(D10)* | **Port 8** *(D8)* | **Port 9** *(D9)* |
| **Rear-Right (RR)** | **Port 7** *(D7)* | **Port 5** *(D5)* | **Port 6** *(D6)* |
| **Rear-Left (RL)** | **Port 12** *(D12)* | **Port 11** *(D11)* | **Port 13** *(D13)* |

---

## ESP32-C6 Hardware Pinout

| Function | XIAO Pin | ESP32-C6 GPIO | Connected Device |
| :--- | :---: | :---: | :--- |
| **I2C SDA** | `D4` | `GPIO 22` | PCA9685 (`0x40`) & OLED SSD1306 (`0x3C`) |
| **I2C SCL** | `D5` | `GPIO 23` | PCA9685 (`0x40`) & OLED SSD1306 (`0x3C`) |
| **Buzzer** | `D3` | `GPIO 21` | Passive Buzzer (Expansion Board) |

---

## Web UI & Calibration Usage

1. Power on the robot.
2. Ensure your computer/phone is on the same network (**`AKR Home`**) or connect to SoftAP (**`ARK-BOT-AP`**, Pass: `12345678`).
3. Open browser:
   - Direct: [`http://arkbot.local`](http://arkbot.local)
   - Or via IP shown on the robot's OLED screen (e.g. `http://192.168.1.xxx`).
4. **Interactive 2D Visual Simulation**: Shows real-time animated limb motion as sliders are adjusted.
5. **Servo Power Controls**:
   - `⚡ Start All Servos`: Energizes all 12 PWM channels.
   - `🛑 Stop All (Release)`: Sets PWM duty cycle to 0, allowing you to freely adjust horns by hand without motor strain or heat.
   - Per-leg and individual joint `ON / OFF` power switches.
   - `🎯 Center All (90°)` and `🌊 Wave Calibration` triggers.
