#!/usr/bin/env python3
"""
ARK-BOT Companion GUI - Locomotion Commander Panel
Provides high-level quadruped action triggers (gait locomotion, 180° turns, postures, gestures).
"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QGridLayout,
    QPushButton, QLabel, QSlider, QSpinBox, QGroupBox, QFrame
)
from PySide6.QtCore import Qt, Signal

try:
    from horizon_widget import ArtificialHorizonCard
except ImportError:
    from ..horizon_widget import ArtificialHorizonCard


class CommanderPanel(QWidget):
    """
    Control panel for high-level robot locomotion, stance, gestures, and attitude instrument.
    """
    action_requested = Signal(str, int, float)  # (action_name, steps, speed)
    height_requested = Signal(str)              # (height_profile: "low" | "normal" | "high")

    def __init__(self, telemetry_client=None, parent=None):
        super().__init__(parent)
        self.telemetry_client = telemetry_client
        self.current_walk_height = "normal"
        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(12, 12, 12, 12)
        main_layout.setSpacing(14)

        # 0. Artificial Horizon Attitude Instrument
        self.horizon_card = ArtificialHorizonCard(self.telemetry_client, self)
        main_layout.addWidget(self.horizon_card)

        # 1. Postures & Emergency Stop
        posture_group = QGroupBox("CORE POSTURES & ELEVATION")
        posture_layout = QGridLayout(posture_group)
        posture_layout.setSpacing(10)

        self.btn_stand = QPushButton("🧍 STAND (-100mm)")
        self.btn_stand.setObjectName("btn_stand")
        self.btn_stand.clicked.connect(lambda: self._trigger_action("stand"))

        self.btn_stand_high = QPushButton("🦒 STAND HIGH (-130mm)")
        self.btn_stand_high.setObjectName("btn_stand_high")
        self.btn_stand_high.setStyleSheet(
            "background-color: #0f766e; border: 1px solid #14b8a6; color: #ccfbf1; font-weight: bold;"
        )
        self.btn_stand_high.clicked.connect(lambda: self._trigger_action("stand_high"))

        self.btn_sit = QPushButton("🪑 SIT (-56mm)")
        self.btn_sit.setObjectName("btn_sit")
        self.btn_sit.clicked.connect(lambda: self._trigger_action("sit"))

        self.btn_stop = QPushButton("🛑 EMERGENCY STOP")
        self.btn_stop.setObjectName("btn_stop")
        self.btn_stop.clicked.connect(lambda: self._trigger_action("stop"))

        posture_layout.addWidget(self.btn_stand, 0, 0)
        posture_layout.addWidget(self.btn_stand_high, 0, 1)
        posture_layout.addWidget(self.btn_sit, 1, 0)
        posture_layout.addWidget(self.btn_stop, 1, 1)
        main_layout.addWidget(posture_group)

        # 2. Locomotion D-Pad with Dynamic Walking Height Selection
        dpad_group = QGroupBox("LOCOMOTION D-PAD (DYNAMIC WALKING HEIGHT)")
        dpad_layout = QGridLayout(dpad_group)
        dpad_layout.setSpacing(10)

        # Walking Height Selector Sub-bar
        height_container = QWidget()
        height_layout = QHBoxLayout(height_container)
        height_layout.setContentsMargins(0, 0, 0, 6)
        height_layout.setSpacing(8)

        lbl_hgt = QLabel("Walk Height:")
        lbl_hgt.setStyleSheet("color: #38bdf8; font-size: 11px; font-weight: bold; text-transform: uppercase; letter-spacing: 0.5px;")
        height_layout.addWidget(lbl_hgt)

        self.btn_hgt_low = QPushButton("🔻 Low (-80mm)")
        self.btn_hgt_low.setObjectName("btn_hgt_low")
        self.btn_hgt_low.clicked.connect(lambda: self._set_walk_height("low"))

        self.btn_hgt_normal = QPushButton("🚶 Normal (-100mm)")
        self.btn_hgt_normal.setObjectName("btn_hgt_normal")
        self.btn_hgt_normal.clicked.connect(lambda: self._set_walk_height("normal"))

        self.btn_hgt_high = QPushButton("🔺 High (-125mm)")
        self.btn_hgt_high.setObjectName("btn_hgt_high")
        self.btn_hgt_high.clicked.connect(lambda: self._set_walk_height("high"))

        for b in [self.btn_hgt_low, self.btn_hgt_normal, self.btn_hgt_high]:
            b.setMinimumHeight(32)
            b.setCursor(Qt.PointingHandCursor)

        height_layout.addWidget(self.btn_hgt_low)
        height_layout.addWidget(self.btn_hgt_normal)
        height_layout.addWidget(self.btn_hgt_high)
        dpad_layout.addWidget(height_container, 0, 0, 1, 3)

        # D-Pad Movement Buttons
        self.btn_fwd = QPushButton("▲ FORWARD")
        self.btn_fwd.setObjectName("btn_fwd")
        self.btn_fwd.clicked.connect(lambda: self._trigger_action("forward"))

        self.btn_left = QPushButton("◄ TURN LEFT")
        self.btn_left.setObjectName("btn_left")
        self.btn_left.clicked.connect(lambda: self._trigger_action("turn_left"))

        self.btn_center = QPushButton("■ BRAKE")
        self.btn_center.setObjectName("btn_brake")
        self.btn_center.clicked.connect(lambda: self._trigger_action("stop"))

        self.btn_right = QPushButton("TURN RIGHT ►")
        self.btn_right.setObjectName("btn_right")
        self.btn_right.clicked.connect(lambda: self._trigger_action("turn_right"))

        self.btn_back = QPushButton("▼ BACKWARD")
        self.btn_back.setObjectName("btn_back")
        self.btn_back.clicked.connect(lambda: self._trigger_action("backward"))

        # Grid placement
        dpad_layout.addWidget(self.btn_fwd, 1, 1)
        dpad_layout.addWidget(self.btn_left, 2, 0)
        dpad_layout.addWidget(self.btn_center, 2, 1)
        dpad_layout.addWidget(self.btn_right, 2, 2)
        dpad_layout.addWidget(self.btn_back, 3, 1)

        for btn in [self.btn_fwd, self.btn_left, self.btn_center, self.btn_right, self.btn_back]:
            btn.setMinimumHeight(42)

        self._refresh_height_ui()
        main_layout.addWidget(dpad_group)

        # 3. 180° U-Turn Rotation Buttons
        turn180_group = QGroupBox("🔄 180° REVERSE ROTATION (U-TURN)")
        turn180_layout = QHBoxLayout(turn180_group)
        turn180_layout.setSpacing(12)

        self.btn_turn180_left = QPushButton("⟲ 180° TURN LEFT")
        self.btn_turn180_left.setObjectName("btn_turn180_left")
        self.btn_turn180_left.setMinimumHeight(42)
        self.btn_turn180_left.setStyleSheet(
            "background-color: #1e1b4b; border: 1px solid #818cf8; color: #c7d2fe; font-weight: bold; font-size: 13px;"
        )
        self.btn_turn180_left.clicked.connect(lambda: self._trigger_action("turn_180_left", custom_steps=8))

        self.btn_turn180_right = QPushButton("⟳ 180° TURN RIGHT")
        self.btn_turn180_right.setObjectName("btn_turn180_right")
        self.btn_turn180_right.setMinimumHeight(42)
        self.btn_turn180_right.setStyleSheet(
            "background-color: #1e1b4b; border: 1px solid #818cf8; color: #c7d2fe; font-weight: bold; font-size: 13px;"
        )
        self.btn_turn180_right.clicked.connect(lambda: self._trigger_action("turn_180_right", custom_steps=8))

        turn180_layout.addWidget(self.btn_turn180_left)
        turn180_layout.addWidget(self.btn_turn180_right)
        main_layout.addWidget(turn180_group)

        # 4. Interactive Gestures
        gesture_group = QGroupBox("SPECIAL GESTURES")
        gesture_layout = QHBoxLayout(gesture_group)
        gesture_layout.setSpacing(10)

        self.btn_shake = QPushButton("🤝 HAND SHAKE")
        self.btn_shake.setObjectName("btn_shake")
        self.btn_shake.clicked.connect(lambda: self._trigger_action("hand_shake"))

        self.btn_wave = QPushButton("👋 HAND WAVE")
        self.btn_wave.setObjectName("btn_wave")
        self.btn_wave.clicked.connect(lambda: self._trigger_action("hand_wave"))

        gesture_layout.addWidget(self.btn_shake)
        gesture_layout.addWidget(self.btn_wave)
        main_layout.addWidget(gesture_group)

        # 5. Gait Parameters (Steps & Speed)
        params_group = QGroupBox("GAIT PARAMETERS")
        params_layout = QVBoxLayout(params_group)

        # Step count
        step_row = QHBoxLayout()
        step_lbl = QLabel("Step Cycles:")
        self.step_spin = QSpinBox()
        self.step_spin.setRange(1, 20)
        self.step_spin.setValue(1)
        step_row.addWidget(step_lbl)
        step_row.addWidget(self.step_spin)
        params_layout.addLayout(step_row)

        # Speed slider
        speed_row = QHBoxLayout()
        speed_lbl = QLabel("Gait Speed Multiplier:")
        self.speed_slider = QSlider(Qt.Horizontal)
        self.speed_slider.setRange(5, 20)
        self.speed_slider.setValue(10)
        self.speed_val_lbl = QLabel("1.0x")

        self.speed_slider.valueChanged.connect(
            lambda v: self.speed_val_lbl.setText(f"{v/10.0:.1f}x")
        )

        speed_row.addWidget(speed_lbl)
        speed_row.addWidget(self.speed_slider)
        speed_row.addWidget(self.speed_val_lbl)
        params_layout.addLayout(speed_row)

        main_layout.addWidget(params_group)
        main_layout.addStretch()

    def _set_walk_height(self, height_profile):
        self.current_walk_height = height_profile
        self._refresh_height_ui()
        self.height_requested.emit(height_profile)
        self.action_requested.emit(f"set_height_{height_profile}", 1, 1.0)

    def update_walk_height(self, height_profile):
        if height_profile and height_profile != self.current_walk_height:
            self.current_walk_height = height_profile.lower()
            self._refresh_height_ui()

    def _refresh_height_ui(self):
        style_active = "background-color: #0284c7; border: 1px solid #00f0ff; color: #ffffff; font-weight: bold; border-radius: 6px;"
        style_inactive = "background-color: #1e293b; border: 1px solid #334155; color: #94a3b8; font-weight: 600; border-radius: 6px;"

        self.btn_hgt_low.setStyleSheet(style_active if self.current_walk_height == "low" else style_inactive)
        self.btn_hgt_normal.setStyleSheet(style_active if self.current_walk_height == "normal" else style_inactive)
        self.btn_hgt_high.setStyleSheet(style_active if self.current_walk_height == "high" else style_inactive)

    def _trigger_action(self, action_name, custom_steps=None):
        steps = custom_steps if custom_steps is not None else self.step_spin.value()
        speed = self.speed_slider.value() / 10.0
        self.action_requested.emit(action_name, steps, speed)

    def update_telemetry(self, data: dict):
        if not data:
            return
        if hasattr(self, 'horizon_card') and self.horizon_card:
            self.horizon_card.update_telemetry(data)
        if "walk_height" in data:
            self.update_walk_height(data["walk_height"])
