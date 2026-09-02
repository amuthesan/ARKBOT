# Changelog

All notable changes to the **ARK-BOT** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [v0.1.1] - 2026-09-02

### Added
- **Seeed Studio XIAO ESP32-C6 RF Antenna Switching**:
  - Software-controlled RF switch via GPIO 3 (Power Enable) and GPIO 14 (Antenna Select).
  - Runtime toggle between **Internal Ceramic Antenna** and **External U.FL / IPEX Antenna** directly from Web UI header.
  - Persistent NVS storage for antenna preference across power cycles.
  - Live RSSI (dBm) signal strength reporting in Web UI and OLED cycling display.
- **Hardware Architecture Modernization**:
  - Migrated core controller from Arduino Nano to **Seeed Studio XIAO ESP32-C6** (160MHz RISC-V, Wi-Fi 6, BLE 5).
  - Integrated **PCA9685 16-Channel I2C PWM Servo Driver** for dedicated 5V/6V servo power and jitter-free pulse generation.
  - Direct 1-to-1 port mapping matching original Nano $D_2 \dots D_{13}$ connections to PCA9685 Channels $2 \dots 13$.
- **Web UI & Embedded Server**:
  - Embedded high-performance, dark-mode cyber robotics Web UI stored directly in flash memory (`PROGMEM`).
  - **Wi-Fi Station Mode**: Automated connection to local network (`AKR Home`) with mDNS hostname ([`http://arkbot.local`](http://arkbot.local)).
  - **SoftAP Fallback**: Broadcasts `ARK-BOT-AP` (`192.168.4.1`) when out of home Wi-Fi range.
- **Visual Kinematics Engine**:
  - Multi-perspective visualization: **3D Isometric**, **Top-Down (Footprint)**, and **Side Elevation**.
  - Smooth mechanical LERP interpolation for realistic servo speed dynamics.
  - Symmetrical $90^\circ$ neutral calibration stance matching physical mechanical horn alignment.
  - Ground contact ripple effects and projected limb shadows.
- **Power Management (3-Tier Start / Stop / Release)**:
  - Master Start / Release buttons to toggle holding torque across all 12 servos.
  - Per-leg power switches (`PWR ON / PWR OFF`) for isolating single limbs.
  - Individual joint power switches (`ON / OFF`) to safely adjust horns by hand with zero holding current.
- **Audio & OLED Telemetry**:
  - Native ESP32 LEDC PWM buzzer driver on pin `D3` (`GPIO 21`) with melodic boot, step tick, completion, and warning chimes.
  - 0.96" SSD1306 OLED display dashboard showing live angle matrix, heartbeat indicator, mode, and assigned IP address.
  - Hardware probing for PCA9685 at boot and runtime with dedicated OLED alert screen and auto-reconnect recovery.
- **Brand Assets & Developer Attribution**:
  - Processed and generated high-DPI transparent white and black brand logos ($128\times128$ and $256\times256$).
  - Embedded glowing brand logo in Web UI header navigation and central robot chassis reactor plate.
  - Added engineering credit footer for Amuthesan / Ark Technology.

### Changed
- Rebranded project from **UAZ** to **ARK-BOT**.
- Replaced direct GPIO bit-banging (`Servo.h`) with hardware-timed I2C PWM (`Adafruit_PWMServoDriver`).
- Updated `README.md` with port-by-port wiring tables, hardware schematics, and connection instructions.

### Removed
- Removed placeholder Stand and Sit pose macros in favor of calibrated neutral midpoint alignment ($90^\circ$).

---

## [v0.1.0] - 2026-09-01
- Initial legacy port baseline from original Regis Hsu Arduino Nano quadruped robot code.
