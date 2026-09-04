# ARK-BOT Quadruped Robot Controller

High-performance quadruped robot controller firmware built for the **Seeed Studio XIAO ESP32-C6** with **PCA9685 16-Channel I2C PWM Servo Driver**, **0.96" SSD1306 OLED Display**, **LEDC Buzzer**, **Switchable Internal/External Antenna**, and an embedded **Wi-Fi 6 Web UI**.

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
| **12** | **Rear-Left (RL)** | **Tibia (Calf)** | `D12` |
| **13** | **Rear-Left (RL)** | **Coxa (Hip)** | `D13` |
| **14** | *(Unused)* | — | — |
| **15** | *(Unused)* | — | — |

---

### Leg & Joint Mapping Summary

| Leg | Coxa (Hip) | Femur (Thigh) | Tibia (Calf) |
| :--- | :---: | :---: | :---: |
| **Front-Right (FR)** | **Port 4** *(D4)* | **Port 2** *(D2)* | **Port 3** *(D3)* |
| **Rear-Right (RR)** | **Port 7** *(D7)* | **Port 5** *(D5)* | **Port 6** *(D6)* |
| **Front-Left (FL)** | **Port 10** *(D10)* | **Port 8** *(D8)* | **Port 9** *(D9)* |
| **Rear-Left (RL)** | **Port 13** *(D13)* | **Port 11** *(D11)* | **Port 12** *(D12)* |

---

## ESP32-C6 Hardware Pinout

| Function | XIAO Pin | ESP32-C6 GPIO | Connected Device |
| :--- | :---: | :---: | :--- |
| **I2C SDA** | `D4` | `GPIO 22` | PCA9685 (`0x40`) & OLED SSD1306 (`0x3C`) |
| **I2C SCL** | `D5` | `GPIO 23` | PCA9685 (`0x40`) & OLED SSD1306 (`0x3C`) |
| **Buzzer** | `D3` | `GPIO 21` | Passive Buzzer (Expansion Board) |
| **RF Switch Power** | — | `GPIO 3` | Onboard RF Switch Power Enable (Active LOW) |
| **Antenna Select** | — | `GPIO 14` | LOW = Onboard Ceramic, HIGH = External U.FL/IPEX |

---

## RF Antenna Selection (Internal Ceramic vs. External IPEX)

The Seeed Studio XIAO ESP32-C6 features an integrated software-controlled RF switch:
* **Internal Antenna (`📡 Int`)**: Uses the onboard ceramic antenna.
* **External Antenna (`🛰️ Ext`)**: Routes RF signals through the U.FL / IPEX antenna connector for extended range.
* **Persistent Storage**: Selected antenna preference is automatically saved to Non-Volatile Storage (NVS) and restored upon reboot.
* **Live Telemetry**: Real-time RSSI signal strength (dBm) is reported on the Web UI and OLED dashboard.

---

## Web UI & Multi-Page Navigation
 
 1. Power on the robot.
 2. Ensure your computer/phone is on the same network (**`AKR Home`**) or connect to the fallback SoftAP (**`ARK-BOT-AP`**, Pass: `12345678`).
 3. Open your browser:
    - **🎮 Action Commander (Homepage)**: [`http://arkbot.local/`](http://arkbot.local/)
    - **🎯 Visual Kinematics Calibrator**: [`http://arkbot.local/calib`](http://arkbot.local/calib)
    - **⚙️ Wi-Fi & System Setup**: [`http://arkbot.local/setup`](http://arkbot.local/setup)
    - Or via IP shown on the robot's OLED screen (e.g. `http://192.168.1.xxx`).
 
 ### 🎮 Action Commander (`/`)
 - **Directional Gait D-Pad**: Click on-screen buttons for **Forward**, **Backward**, **Turn Left**, **Turn Right**, and **Stop/Rest**.
 - **Postures & Social Gestures**: One-click **Stand Up** ($Z = -100\text{ mm}$), **Sit Down** ($Z = -56\text{ mm}$), **Hand Shake**, and **Hand Wave**.
 - **Gait Customization**: Select step counts ($1, 2, 3, 5, 10$) and speed multipliers ($1.0\times, 1.5\times, 2.0\times$).
 - **Live 3D Kinematics Visualizer**: Real-time 3D simulation displaying robot movements and limb articulation.
 
 ### 🎯 Visual Calibrator (`/calib`)
 - **Interactive Kinematics Simulator**: `3D Isometric`, `Top-Down`, and `Side Elevation` perspectives with smooth mechanical LERP animation.
 - **Master Power & Neutral Alignment**: Master Start/Stop holding torque buttons, Center All ($90^\circ$), and Wave Calibration.
 - **Individual Joint Sliders**: Independent power toggles and sliders for all 12 leg joints across 4 legs.
 - **RF Antenna Switcher**: Direct toggle between `📡 Int` (Ceramic) and `🛰️ Ext` (External IPEX) with live RSSI reporting.
 
 ### ⚙️ Wi-Fi & System Setup (`/setup`)
 - **Wi-Fi Scanner**: One-click 2.4GHz network scan with live signal strength meters, channel numbers, and security modes.
 - **Credential Configuration**: Quick-select network from scan results, input Wi-Fi password with show/hide toggle, and save to persistent NVS storage across reboots.
 - **Telemetry Dashboard**: Live connection status, Station IP, SoftAP IP, MAC address, mDNS URL, and system uptime.
 - **Device Management**: Safely restart the ESP32-C6 controller or reset Wi-Fi configuration with automatic reconnection timer.


