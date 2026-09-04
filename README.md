# ARK-BOT Quadruped Robot Controller

High-performance quadruped robot controller firmware built for the **Seeed Studio XIAO ESP32-C6** with **PCA9685 16-Channel I2C PWM Servo Driver**, **0.96" SSD1306 OLED Display**, **LEDC Buzzer**, **Switchable Internal/External Antenna**, and an embedded **Wi-Fi 6 Web UI**.

> **System Architecture**: High-efficiency FreeRTOS-powered quadruped controller with dedicated **PCA9685 16-Channel 12-bit I2C PWM driver** (powered at 5V/6V), 32-bit RISC-V **XIAO ESP32-C6**, live vector kinematics visualizer, Over-The-Air (OTA) firmware updates, and dual connectivity (USB Serial & Wi-Fi 6).

---

## PCA9685 16-Channel Servo Wiring Guide

All 12 servos connect directly to the **PCA9685 16-Channel I2C PWM Driver** using standard 3-pin servo headers (Signal, V+, GND):

### Port-by-Port Wiring Table

| PCA9685 Channel | Leg | Joint |
| :---: | :--- | :--- |
| **0** | *(Unused)* | — |
| **1** | *(Unused)* | — |
| **2** | **Front-Right (FR)** | **Femur (Thigh)** |
| **3** | **Front-Right (FR)** | **Tibia (Calf)** |
| **4** | **Front-Right (FR)** | **Coxa (Hip)** |
| **5** | **Rear-Right (RR)** | **Femur (Thigh)** |
| **6** | **Rear-Right (RR)** | **Tibia (Calf)** |
| **7** | **Rear-Right (RR)** | **Coxa (Hip)** |
| **8** | **Front-Left (FL)** | **Femur (Thigh)** |
| **9** | **Front-Left (FL)** | **Tibia (Calf)** |
| **10** | **Front-Left (FL)** | **Coxa (Hip)** |
| **11** | **Rear-Left (RL)** | **Femur (Thigh)** |
| **12** | **Rear-Left (RL)** | **Tibia (Calf)** |
| **13** | **Rear-Left (RL)** | **Coxa (Hip)** |
| **14** | *(Unused)* | — |
| **15** | *(Unused)* | — |

---

### Leg & Joint Mapping Summary

| Leg | Coxa (Hip) | Femur (Thigh) | Tibia (Calf) |
| :--- | :---: | :---: | :---: |
| **Front-Right (FR)** | **Channel 4** | **Channel 2** | **Channel 3** |
| **Rear-Right (RR)** | **Channel 7** | **Channel 5** | **Channel 6** |
| **Front-Left (FL)** | **Channel 10** | **Channel 8** | **Channel 9** |
| **Rear-Left (RL)** | **Channel 13** | **Channel 11** | **Channel 12** |

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
    - **🚀 OTA Firmware Update**: [`http://arkbot.local/update`](http://arkbot.local/update)
    - Or via IP shown on the robot's OLED screen (e.g. `http://192.168.1.xxx`).
 
 ### 🎮 Action Commander (`/`)
 - **Directional Gait D-Pad**: Click on-screen buttons for **Forward**, **Backward**, **Turn Left**, **Turn Right**, **180° Reverse Turn**, and **Stop/Rest**.
 - **Dynamic Walking Height**: Select between **🔻 Low (-80mm)**, **🚶 Normal (-100mm)**, and **🔺 High (-125mm)** walking height presets.
 - **Postures & Social Gestures**: One-click **Stand Up**, **Stand High**, **Sit Down**, **Hand Shake**, and **Hand Wave**.
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

 ### 🚀 OTA Firmware Update (`/update`)
 - **Browser Firmware Flash**: Drag & drop or select `.bin` binary build files to flash the ESP32-C6 over Wi-Fi without USB cables.
 - **Live Upload Progress**: Real-time progress bar with upload speed, byte count, and percentage indicator.
 - **Automated Reboot**: Automatic reboot sequence and web interface reconnect after flashing completes.

---

## 🖥️ Python Desktop Companion GUI

A cross-platform desktop application built with **PySide6** and vector graphics for low-latency robot control, live 3D kinematics telemetry, and joint calibration.

### ✨ Features
- **Dual Connectivity**: Connect via **USB Serial** (`115200` baud auto-streaming) or **Wi-Fi** (HTTP polling at `http://arkbot.local`).
- **Interactive 3D Kinematics Visualizer**: Real-time vector-rendered 3D viewport of the robot chassis and 4 articulated limbs with orbit, pan, and zoom camera controls.
- **Locomotion Commander**: D-Pad gait controller, posture switches (Stand/Sit/Stop), gestures (Hand Shake/Wave), and step/speed parameters.
- **Servo Joint Calibrator**: 12 individual servo angle sliders ($0^\circ-180^\circ$) with live readouts, Center All ($90^\circ$ Neutral), and Master Power toggles.
- **Diagnostics & Telemetry**: Real-time $(X, Y, Z)$ Cartesian limb matrix, raw servo angle readouts, PCA9685 health, packet rate counter, and serial communication log.

### 🚀 Running the Companion GUI
```bash
# 1. Install dependencies
pip install -r gui/requirements.txt

# 2. Launch application
python3 gui/run.py
```
