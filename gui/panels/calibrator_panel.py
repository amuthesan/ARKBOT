#!/usr/bin/env python3
"""
ARK-BOT Companion GUI - Servo Calibrator Panel
Provides precise joint angle sliders (0-180°), channel mapping indicators,
power toggles, and neutral calibration triggers.
"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QSlider, QGroupBox, QTabWidget
)
from PySide6.QtCore import Qt, Signal

# Channel mappings: SERVO_CHANNELS[4][3]
CH_NAMES = [
    ["CH 4 (Coxa / Hip)", "CH 2 (Femur / Thigh)", "CH 3 (Tibia / Knee)"],    # Leg 0: FR
    ["CH 7 (Coxa / Hip)", "CH 5 (Femur / Thigh)", "CH 6 (Tibia / Knee)"],    # Leg 1: RR
    ["CH 10 (Coxa / Hip)", "CH 8 (Femur / Thigh)", "CH 9 (Tibia / Knee)"],   # Leg 2: FL
    ["CH 13 (Coxa / Hip)", "CH 11 (Femur / Thigh)", "CH 12 (Tibia / Knee)"], # Leg 3: RL
]

LEG_NAMES = ["Front-Right (FR)", "Rear-Right (RR)", "Front-Left (FL)", "Rear-Left (RL)"]


class CalibratorPanel(QWidget):
    """
    Control panel for low-level individual servo angle trimming and calibration.
    """
    servo_changed = Signal(int, int, int)  # (leg, joint, angle)
    power_toggled = Signal(bool)          # (state)
    init_requested = Signal(str)          # (target)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._updating_from_telemetry = False
        self.sliders = [[None for _ in range(3)] for _ in range(4)]
        self.labels = [[None for _ in range(3)] for _ in range(4)]
        self.master_power_state = True
        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(12, 12, 12, 12)
        main_layout.setSpacing(14)

        # 1. Calibration Toolbar
        tool_group = QGroupBox("CALIBRATION & POWER TOOLS")
        tool_layout = QHBoxLayout(tool_group)

        self.btn_center_all = QPushButton("🎯 CENTER ALL (90° NEUTRAL)")
        self.btn_center_all.setObjectName("btn_center_all")
        self.btn_center_all.clicked.connect(lambda: self.init_requested.emit("all"))

        self.btn_wave_test = QPushButton("🌊 WAVE TEST")
        self.btn_wave_test.clicked.connect(lambda: self.init_requested.emit("wave"))

        self.btn_pwr = QPushButton("⚡ POWER: ON")
        self.btn_pwr.setObjectName("btn_pwr_on")
        self.btn_pwr.clicked.connect(self._toggle_master_power)

        tool_layout.addWidget(self.btn_center_all)
        tool_layout.addWidget(self.btn_wave_test)
        tool_layout.addWidget(self.btn_pwr)
        main_layout.addWidget(tool_group)

        # 2. Leg Sub-Tabs
        self.tabs = QTabWidget()
        for leg_idx in range(4):
            leg_widget = QWidget()
            leg_layout = QVBoxLayout(leg_widget)
            leg_layout.setSpacing(12)

            for joint_idx in range(3):
                row_box = QGroupBox(CH_NAMES[leg_idx][joint_idx])
                row_layout = QHBoxLayout(row_box)

                lbl_val = QLabel("90°")
                lbl_val.setFixedWidth(45)
                lbl_val.setAlignment(Qt.AlignCenter)
                lbl_val.setStyleSheet("font-weight: bold; color: #00f0ff; font-size: 14px;")

                slider = QSlider(Qt.Horizontal)
                slider.setRange(0, 180)
                slider.setValue(90)
                slider.setTickPosition(QSlider.TicksBelow)
                slider.setTickInterval(15)

                def make_handler(l, j, s, lv):
                    def handler(val):
                        lv.setText(f"{val}°")
                        if not self._updating_from_telemetry:
                            self.servo_changed.emit(l, j, val)
                    return handler

                slider.valueChanged.connect(make_handler(leg_idx, joint_idx, slider, lbl_val))

                btn_90 = QPushButton("90°")
                btn_90.setFixedWidth(50)
                btn_90.clicked.connect(lambda checked, s=slider: s.setValue(90))

                row_layout.addWidget(slider)
                row_layout.addWidget(lbl_val)
                row_layout.addWidget(btn_90)
                leg_layout.addWidget(row_box)

                self.sliders[leg_idx][joint_idx] = slider
                self.labels[leg_idx][joint_idx] = lbl_val

            leg_layout.addStretch()
            self.tabs.addTab(leg_widget, LEG_NAMES[leg_idx])

        main_layout.addWidget(self.tabs)

    def _toggle_master_power(self):
        self.master_power_state = not self.master_power_state
        if self.master_power_state:
            self.btn_pwr.setText("⚡ POWER: ON")
            self.btn_pwr.setObjectName("btn_pwr_on")
        else:
            self.btn_pwr.setText("🔌 POWER: OFF")
            self.btn_pwr.setObjectName("btn_pwr_off")
        self.btn_pwr.style().unpolish(self.btn_pwr)
        self.btn_pwr.style().polish(self.btn_pwr)
        self.power_toggled.emit(self.master_power_state)

    def update_telemetry(self, telem):
        if not telem or "angles" not in telem:
            return

        angles = telem["angles"]
        if isinstance(angles, list) and len(angles) == 4:
            self._updating_from_telemetry = True
            for l in range(4):
                if isinstance(angles[l], list) and len(angles[l]) == 3:
                    for j in range(3):
                        slider = self.sliders[l][j]
                        lbl = self.labels[l][j]
                        # Do not override if user is actively dragging slider
                        if slider and lbl and not slider.isSliderDown():
                            val = int(angles[l][j])
                            if slider.value() != val:
                                slider.blockSignals(True)
                                slider.setValue(val)
                                lbl.setText(f"{val}°")
                                slider.blockSignals(False)
            self._updating_from_telemetry = False
