#!/usr/bin/env python3
"""
ARK-BOT Companion GUI - Telemetry Panel & Diagnostics
Displays real-time Cartesian kinematics coordinates, joint matrices,
hardware diagnostics, packet rate, and serial/network log console.
"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QGridLayout,
    QLabel, QGroupBox, QTableWidget, QTableWidgetItem,
    QHeaderView, QTextEdit, QPushButton
)
from PySide6.QtCore import Qt
from PySide6.QtGui import QColor

LEG_LABELS = ["Leg 0: FR", "Leg 1: RR", "Leg 2: FL", "Leg 3: RL"]
LEG_COLORS = ["#00f0ff", "#a855f7", "#10b981", "#f59e0b"]


class TelemetryPanel(QWidget):
    """
    Real-time telemetry and diagnostics view.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.packet_count = 0
        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(12, 12, 12, 12)
        main_layout.setSpacing(14)

        # 1. System Health & Status Metrics
        diag_group = QGroupBox("HARDWARE STATUS & METRICS")
        diag_layout = QGridLayout(diag_group)

        self.lbl_robot = QLabel("Robot: ARK-BOT")
        self.lbl_ver = QLabel("Firmware: --")
        self.lbl_pca = QLabel("PCA9685: Checking...")
        self.lbl_mode = QLabel("Mode: STAND")
        self.lbl_packets = QLabel("Packets: 0")
        self.lbl_vbat = QLabel("Battery: -- V")
        self.lbl_imu = QLabel("IMU (MPU6050): Checking...")
        self.lbl_imu_att = QLabel("Attitude: P: 0.0° R: 0.0°")

        diag_layout.addWidget(self.lbl_robot, 0, 0)
        diag_layout.addWidget(self.lbl_ver, 0, 1)
        diag_layout.addWidget(self.lbl_pca, 0, 2)
        diag_layout.addWidget(self.lbl_mode, 1, 0)
        diag_layout.addWidget(self.lbl_packets, 1, 1)
        diag_layout.addWidget(self.lbl_vbat, 1, 2)
        diag_layout.addWidget(self.lbl_imu, 2, 0)
        diag_layout.addWidget(self.lbl_imu_att, 2, 1, 1, 2)

        main_layout.addWidget(diag_group)

        # 2. Kinematics Coordinate & Angle Table
        table_group = QGroupBox("KINEMATICS SITES & JOINT ANGLES")
        table_layout = QVBoxLayout(table_group)

        self.table = QTableWidget(4, 7)
        self.table.setHorizontalHeaderLabels([
            "Limb", "X (mm)", "Y (mm)", "Z (mm)",
            "Coxa (°)", "Femur (°)", "Tibia (°)"
        ])
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.verticalHeader().setVisible(False)
        self.table.setEditTriggers(QTableWidget.NoEditTriggers)

        for row in range(4):
            item_limb = QTableWidgetItem(LEG_LABELS[row])
            item_limb.setForeground(QColor(LEG_COLORS[row]))
            self.table.setItem(row, 0, item_limb)
            for col in range(1, 7):
                item = QTableWidgetItem("0.0")
                item.setTextAlignment(Qt.AlignCenter)
                self.table.setItem(row, col, item)

        table_layout.addWidget(self.table)
        main_layout.addWidget(table_group)

        # 3. Communication Log Console (Capped to 100 blocks to prevent memory/layout lag)
        log_group = QGroupBox("COMMUNICATION CONSOLE")
        log_layout = QVBoxLayout(log_group)

        self.log_text = QTextEdit()
        self.log_text.setReadOnly(True)
        self.log_text.document().setMaximumBlockCount(120)
        self.log_text.setStyleSheet(
            "background-color: #070a0f; color: #38bdf8; font-family: monospace; font-size: 11px;"
        )

        btn_row = QHBoxLayout()
        btn_clear = QPushButton("Clear Console")
        btn_clear.setFixedWidth(120)
        btn_clear.clicked.connect(self.log_text.clear)
        btn_row.addStretch()
        btn_row.addWidget(btn_clear)

        log_layout.addWidget(self.log_text)
        log_layout.addLayout(btn_row)
        main_layout.addWidget(log_group)

    def append_log(self, level, message):
        color = "#94a3b8"
        if level == "ERROR":
            color = "#ef4444"
        elif level == "WARN":
            color = "#f59e0b"
        elif level == "TX":
            color = "#a855f7"
        elif level == "RX_MSG":
            color = "#10b981"
        elif level == "INFO":
            color = "#00f0ff"

        self.log_text.append(f'<span style="color:{color}">[{level}] {message}</span>')
        sb = self.log_text.verticalScrollBar()
        sb.setValue(sb.maximum())

    def update_telemetry(self, telem):
        if not telem:
            return

        self.packet_count += 1
        self.lbl_packets.setText(f"Packets: {self.packet_count}")

        if "version" in telem:
            self.lbl_ver.setText(f"Firmware: {telem['version']}")

        if "mode" in telem:
            self.lbl_mode.setText(f"Mode: {telem['mode']}")

        if "pca" in telem:
            ready = bool(telem["pca"])
            self.lbl_pca.setText("PCA9685: READY" if ready else "PCA9685: OFFLINE")
            self.lbl_pca.setStyleSheet("color: #10b981;" if ready else "color: #ef4444; font-weight: bold;")

        if "vbat" in telem:
            try:
                vbat = float(telem["vbat"])
                self.lbl_vbat.setText(f"Battery: {vbat:.2f} V")
                if vbat < 6.8:
                    self.lbl_vbat.setStyleSheet("color: #ef4444; font-weight: bold;")
                elif vbat < 7.4:
                    self.lbl_vbat.setStyleSheet("color: #f59e0b; font-weight: bold;")
                else:
                    self.lbl_vbat.setStyleSheet("color: #10b981; font-weight: bold;")
            except (ValueError, TypeError):
                pass

        if "imu" in telem and isinstance(telem["imu"], dict):
            imu = telem["imu"]
            ready = bool(imu.get("ready", False))
            self.lbl_imu.setText("IMU (MPU6050): ONLINE" if ready else "IMU (MPU6050): OFFLINE")
            self.lbl_imu.setStyleSheet("color: #10b981;" if ready else "color: #ef4444; font-weight: bold;")
            pitch = float(imu.get("pitch", 0.0))
            roll = float(imu.get("roll", 0.0))
            self.lbl_imu_att.setText(f"Attitude: P: {pitch:+.1f}° | R: {roll:+.1f}°")


        # Update Table Sites (cols 1, 2, 3)
        if "sites" in telem and isinstance(telem["sites"], list):
            for r in range(min(4, len(telem["sites"]))):
                site = telem["sites"][r]
                if isinstance(site, list) and len(site) == 3:
                    item_x = self.table.item(r, 1)
                    item_y = self.table.item(r, 2)
                    item_z = self.table.item(r, 3)
                    if item_x: item_x.setText(f"{site[0]:.1f}")
                    if item_y: item_y.setText(f"{site[1]:.1f}")
                    if item_z: item_z.setText(f"{site[2]:.1f}")

        # Update Table Angles (cols 4, 5, 6)
        if "angles" in telem and isinstance(telem["angles"], list):
            for r in range(min(4, len(telem["angles"]))):
                angles = telem["angles"][r]
                if isinstance(angles, list) and len(angles) == 3:
                    item_c = self.table.item(r, 4)
                    item_f = self.table.item(r, 5)
                    item_t = self.table.item(r, 6)
                    if item_c: item_c.setText(f"{int(angles[0])}°")
                    if item_f: item_f.setText(f"{int(angles[1])}°")
                    if item_t: item_t.setText(f"{int(angles[2])}°")
