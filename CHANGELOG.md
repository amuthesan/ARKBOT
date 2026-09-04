# Changelog

All notable changes to the **ARK-BOT** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v1.0.4] - 2026-09-04

### Added
- **Dynamic Multi-Level Walking Height Locomotion**:
  - Implemented dynamic stance and swing clearance profiles for walking gaits:
    - **🔻 Low / Crouch**: $Z = -80.0\text{ mm}$ (Swing lift $Z = -50.0\text{ mm}$)
    - **🚶 Normal**: $Z = -100.0\text{ mm}$ (Swing lift $Z = -60.0\text{ mm}$)
    - **🔺 High / Tall**: $Z = -125.0\text{ mm}$ (Swing lift $Z = -85.0\text{ mm}$)
  - Gaits (`step_forward`, `step_back`, `turn_left`, `turn_right`, `turn_180_*`) dynamically adapt to the selected walking height in real time.
  - Added smooth stance transitions between heights while standing.
- **Locomotion D-Pad Walking Height Controls**:
  - **Python Companion GUI**: Integrated a dedicated Walk Height segmented selector directly into the **Locomotion D-Pad** box (`[ 🔻 Low (-80mm) ]`, `[ 🚶 Normal (-100mm) ]`, `[ 🔺 High (-125mm) ]`) with active cyber-glow highlights.
  - **WebUI Commander**: Added `Low`, `Normal`, and `High` pills into the Locomotion & Turning card header.
- **Bi-directional Height Telemetry**:
  - Added `"walk_height"` field to Serial and Web JSON status reports for instantaneous two-way GUI sync.

### Changed
- **Standing Detection Threshold**:
  - Updated `is_stand()` threshold from `<= -85.0mm` to `<= -70.0mm` so that Low/Crouch stance ($Z = -80.0\text{ mm}$) is accurately identified as standing.

## [v1.0.3] - 2026-09-04

### Added
- **180° Reverse Turn (U-Turn) Kinematics & Controls**:
  - Implemented `turn_180_left` and `turn_180_right` routines in FreeRTOS `actionTask`.
  - Added dedicated **⟲ 180° TURN LEFT** and **⟳ 180° TURN RIGHT** buttons in the Locomotion Commander panel.
  - Automatically executes an exact 8-step synchronized diagonal-pair gait cycle with ground clearance to rotate the robot a full 180° from front to rear.
- **Python Desktop Companion GUI (`gui/`)**:
  - Built high-performance desktop GUI with **PySide6** and cyber-dark aesthetic.
  - Interactive **3D Vector Kinematics Viewport** with exact forward trigonometric projection ($w, v, \alpha, \beta, \gamma$), 4 articulated legs (Coxa, Femur, Tibia), foot ground shadows, contact points, and mouse orbit/pan/zoom.
  - **Locomotion Commander Panel**: Directional D-Pad, 180° U-turn buttons, posture switches (Stand/Sit/Stop), gestures (Hand Shake/Wave), and step/speed controls.
  - **Joint Calibrator Panel**: Individual $0^\circ-180^\circ$ sliders for 12 servo channels, Center All ($90^\circ$ Neutral), and Master Power toggles.
  - **Diagnostics & Telemetry Panel**: Live Cartesian $(X, Y, Z)$ coordinates, servo angles, packet counter, and communication console.
- **Dual Connectivity & Zero-Latency Engine**:
  - **USB Serial Streaming**: Continuous 20Hz newline-delimited JSON stream over USB Serial with immediate OS buffer flushing (`_ser.flush()`).
  - **Wi-Fi Direct IP Caching**: Resolves `arkbot.local` to direct IP upon connection, dropping network response latency from **5,000ms down to 34ms**.
  - **Zero-Dependency Core**: Uses standard Python libraries (`http.client`, `urllib`, `socket`) + `PySide6` + `pyserial` with no external dependencies.
- **Structured JSON Serial Protocol**:
  - Real-time telemetry: `{"t":..., "robot":"ARK-BOT", "version":"v1.0.3", "pca":true, "mode":"...", "moving":..., "sites":[[x,y,z],...], "angles":[[c,f,t],...], "pwr":[[true,true,true],...]}`
  - Structured bidirectional command execution for actions, individual servos, master power, and calibration.

### Fixed
- **Serial Parsing Latency**:
  - Replaced blocking `Serial.readStringUntil('\n')` in `loop()` with non-blocking character accumulation and `Serial.setTimeout(5)`, making serial command execution sub-millisecond.
- **Wi-Fi Driver Lock Contention & TCP Closing**:
  - Added `Connection: close` headers to `/api/status` responses and throttled `WiFi.RSSI()` and `WiFi.status()` queries to eliminate network stack stalls.
- **Data Key Normalization**:
  - Unified data schemas between Serial (`pca`, `pwr`) and WebUI (`pcaReady`, `enabled`) so all GUI panels update in real time.

---

## [v1.0.1] - 2026-09-04

### Fixed
- **Calibration "Center All (90°)" & Slider Overwrite Issue**:
  - Gated background FreeRTOS kinematics task with `kinematicsActive` flag so entering calibration mode or clicking "Center All (90°)" directly controls PCA9685 PWM registers without interference.
- **Exact Arduino Nano Kinematics Restoration**:
  - Restored exact 1-to-1 hardware channel and leg mapping matching original Arduino Nano layout (0: Front-Right, 1: Rear-Right, 2: Front-Left, 3: Rear-Left) and joint order (Coxa, Femur, Tibia).
  - Ported authentic Nano equations for gaits, spot turns, body shifting, and hand gestures.
- **Live 3D Kinematics Web Visualizer**:
  - Added real-time Cartesian sites telemetry (`"sites": [[x, y, z], ...]`) to `/api/status`.
  - Implemented exact forward kinematics geometry on HTML5 Canvas in `/` Commander to accurately animate all 4 limbs, stance elevation, and foot ground contact pads in real time.
- **Vector Cyber Logo Asset**:
  - Replaced broken raster base64 logo with a high-definition vector SVG cybernetic quadruped emblem across all UI pages.
- **PCA9685 Offline Detection & Safety Guard**:
  - Dynamic I2C connection verification; accurately displays `STATUS: PCA OFFLINE (0x40)` in red when servo driver is disconnected.
  - Added action lockouts and OLED alert (`! PCA9685 OFFLINE !`) when hardware is unpowered.
  - Enabled auto-recovery when PCA9685 power / I2C is reconnected.

---

## [v1.0.0] - 2026-09-04

### Added
- **Full Inverse Kinematics & 3D Trigonometric Engine Ported to ESP32-C6**:
  - Implemented 50Hz (20ms) high-precision FreeRTOS kinematics task executing continuous Cartesian-to-Polar inverse kinematics transformations (`cartesian_to_polar` and `polar_to_servo`).
  - Seamless interpolation and trajectory tracking towards `site_expect` across all 4 legs.
- **Cyber Action Commander Homepage (`/`)**:
  - Homepage served at [`http://arkbot.local/`](http://arkbot.local/) is now the primary **Motion Commander Dashboard**.
  - Directional Motion D-Pad: **FORWARD**, **BACKWARD**, **TURN LEFT**, **TURN RIGHT**, and **STOP / REST**.
  - Posture & Gesture Triggers: **STAND UP** (height -100mm), **SIT DOWN** (rest height -56mm), **HAND SHAKE**, and **HAND WAVE**.
  - Live 3D/2D Kinematics simulator displaying real-time robot pose.
  - Step counter selector ($1, 2, 3, 5, 10$ steps) and Gait speed multiplier ($1.0\times, 1.5\times, 2.0\times$).
- **Asynchronous FreeRTOS Motion Queue**:
  - Dedicated background action worker task executing gaits smoothly without blocking HTTP WebServer requests or UI responsiveness.
  - Instant motion abort / emergency stop support.
- **Unified 3-Page Navigation Header**:
  - Seamless switching across `🎮 Commander` (`/`), `🎯 Calibrator` (`/calib`), and `⚙️ Setup` (`/setup`).
- **REST & Serial Command Protocol**:
  - `POST /api/action`: Execute motion routines via REST API.
  - Legacy Serial command interface (`w <action_mode> <steps>`) supported for HC-06 Bluetooth / Serial UART remote controllers.

### Changed
- Promoted project to **v1.0.0** stable release.
- Homepage `/` now hosts the Action Commander dashboard while calibration is available at `/calib`.

---

## [v0.1.2] - 2026-09-04


### Added
- **Dedicated Setup & Wi-Fi Configuration (`/setup`)**:
  - Available at [`http://arkbot.local/setup`](http://arkbot.local/setup).
  - One-click **2.4GHz Wi-Fi scanning** with visual RSSI signal bars, security status (WPA/WPA2/WPA3/Open), and channel listing.
  - Interactive network selector that auto-fills SSID upon clicking a scanned network.
  - Wi-Fi credentials form with password visibility toggle (show/hide).
  - Persistent **NVS storage** (`ark_wifi` namespace) for saving custom Wi-Fi network credentials across power cycles.
  - Connection & system telemetry dashboard: active mode (STA vs SoftAP), assigned Station IP, SoftAP IP, device MAC address, hostname, RSSI, antenna status, and system uptime.
  - Device management actions: Wi-Fi reconnect, Wi-Fi credentials reset, and controller reboot with automated countdown reconnect overlay.
- **Dedicated Calibration Route (`/calib`)**:
  - Calibration page moved from root `/` to [`http://arkbot.local/calib`](http://arkbot.local/calib).
  - Root route (`/`) automatically redirects (HTTP 302) to `/calib`.
- **Unified Navigation Header**:
  - Cyber-themed navigation tabs (`🎯 Calibrator` and `⚙️ Setup`) integrated across all Web UI pages.
  - Retained RF Antenna toggle widget and live PCA9685/Wi-Fi status badges across pages.
- **REST API Endpoints**:
  - `GET /api/wifi/scan`: Discovers nearby Wi-Fi access points and returns JSON list.
  - `GET /api/wifi/status`: Returns comprehensive network connection status and hardware telemetry.
  - `POST /api/wifi/save`: Saves new Wi-Fi credentials to NVS and triggers connection.
  - `POST /api/wifi/reconnect`: Reconnects to configured Wi-Fi network.
  - `POST /api/wifi/reset`: Clears saved Wi-Fi configuration from NVS.
  - `POST /api/reboot`: Safely reboots the ESP32-C6 controller.

### Changed
- Moved visual kinematics calibrator from root `/` to `/calib`.
- Configured partition table to `min_spiffs.csv` to allocate 1.96MB flash space for multi-page Web UI application.

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
- Initial codebase foundation and quadruped kinematics architecture.
