#!/usr/bin/env python3
"""
ARK-BOT Desktop GUI - Artificial Horizon (Attitude Director Indicator)
Vector-rendered high-precision avionics instrument using PySide6 QPainter.
"""

import math
from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel, QHBoxLayout
from PySide6.QtCore import Qt, QRectF, QPointF
from PySide6.QtGui import (
    QPainter, QColor, QPen, QBrush, QFont, QPolygonF, QRadialGradient
)


class HorizonInstrument(QWidget):
    """Circular Attitude Indicator Dial rendered via vector QPainter."""

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumSize(140, 140)
        self.pitch = 0.0  # degrees (+ = nose up)
        self.roll = 0.0   # degrees (+ = right wing down / roll right)
        self.ready = False

    def update_attitude(self, pitch: float, roll: float, ready: bool = True):
        self.pitch = float(pitch)
        self.roll = float(roll)
        self.ready = bool(ready)
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing, True)
        painter.setRenderHint(QPainter.RenderHint.TextAntialiasing, True)

        w = self.width()
        h = self.height()
        size = min(w, h)
        radius = (size / 2.0) - 6.0
        cx = w / 2.0
        cy = h / 2.0

        # Background Dial Clipping Circle
        dial_rect = QRectF(cx - radius, cy - radius, radius * 2, radius * 2)

        if not self.ready:
            # Offline State
            painter.setPen(QPen(QColor("#1e293b"), 2))
            painter.setBrush(QBrush(QColor("#0f172a")))
            painter.drawEllipse(dial_rect)

            painter.setPen(QPen(QColor("#64748b")))
            font = QFont("Helvetica Neue", 11, QFont.Weight.Bold)
            painter.setFont(font)
            painter.drawText(dial_rect, Qt.AlignmentFlag.AlignCenter, "MPU6050\nOFFLINE")
            return

        # 1. Clip inside Dial Circle
        painter.save()
        from PySide6.QtGui import QPainterPath
        clip_path = QPainterPath()
        clip_path.addEllipse(dial_rect)
        painter.setClipPath(clip_path)

        # 2. Attitude Transformation: Rotate by Roll, Translate by Pitch
        painter.translate(cx, cy)
        painter.rotate(-self.roll)

        px_per_deg = radius / 35.0
        pitch_px = self.pitch * px_per_deg

        # Sky (Cyber Sky Blue)
        sky_rect = QRectF(-radius * 2, -radius * 2 + pitch_px, radius * 4, radius * 2)
        painter.fillRect(sky_rect, QColor("#0284c7"))

        # Earth / Ground (Warm Earth Slate)
        earth_rect = QRectF(-radius * 2, pitch_px, radius * 4, radius * 2)
        painter.fillRect(earth_rect, QColor("#78350f"))

        # Horizon Line
        pen_horizon = QPen(QColor("#ffffff"), 2.5)
        painter.setPen(pen_horizon)
        painter.drawLine(QPointF(-radius * 2, pitch_px), QPointF(radius * 2, pitch_px))

        # Pitch Ladder (+/- 10, 20, 30 deg)
        ladder_font = QFont("Courier", 8, QFont.Weight.Bold)
        painter.setFont(ladder_font)

        for deg in range(-40, 41, 10):
            if deg == 0:
                continue
            line_y = pitch_px - (deg * px_per_deg)
            is_major = (abs(deg) % 20 == 0)
            line_w = 24.0 if is_major else 14.0

            pen_ladder = QPen(QColor(255, 255, 255, 220), 2.0 if is_major else 1.2)
            painter.setPen(pen_ladder)
            painter.drawLine(QPointF(-line_w, line_y), QPointF(line_w, line_y))

            # Pitch labels
            text = str(abs(deg))
            painter.setPen(QPen(QColor(255, 255, 255, 230)))
            painter.drawText(QRectF(line_w + 4, line_y - 8, 24, 16), Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter, text)
            painter.drawText(QRectF(-line_w - 28, line_y - 8, 24, 16), Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter, text)

        painter.restore()

        # 3. Outer Cyber Bezel
        painter.save()
        bezel_pen = QPen(QColor("#00f0ff"), 3.5)
        painter.setPen(bezel_pen)
        painter.setBrush(Qt.BrushStyle.NoBrush)
        painter.drawEllipse(dial_rect)

        # Roll Bank Index Markers (0, +/-30, +/-60 deg)
        marker_pen = QPen(QColor("#00f0ff"), 2.0)
        painter.setPen(marker_pen)
        for angle in [0, -30, 30, -60, 60]:
            rad = math.radians(angle - 90)
            p1 = QPointF(cx + (radius - 2) * math.cos(rad), cy + (radius - 2) * math.sin(rad))
            p2 = QPointF(cx + (radius - 8) * math.cos(rad), cy + (radius - 8) * math.sin(rad))
            painter.drawLine(p1, p2)

        # 4. Fixed Aircraft Reference Symbol (Yellow Crosshair / Wings)
        reticle_pen = QPen(QColor("#facc15"), 3.0)
        reticle_pen.setCapStyle(Qt.PenCapStyle.RoundCap)
        painter.setPen(reticle_pen)
        painter.setBrush(QBrush(QColor("#facc15")))

        # Left Wing
        painter.drawLine(QPointF(cx - 36, cy), QPointF(cx - 14, cy))
        painter.drawLine(QPointF(cx - 14, cy), QPointF(cx - 14, cy + 6))

        # Right Wing
        painter.drawLine(QPointF(cx + 36, cy), QPointF(cx + 14, cy))
        painter.drawLine(QPointF(cx + 14, cy), QPointF(cx + 14, cy + 6))

        # Center Reference Pip
        painter.drawEllipse(QRectF(cx - 3, cy - 3, 6, 6))

        # Top Zero Roll Pointer
        pointer = QPolygonF([
            QPointF(cx, cy - radius + 3),
            QPointF(cx - 5, cy - radius + 11),
            QPointF(cx + 5, cy - radius + 11)
        ])
        painter.drawPolygon(pointer)

        painter.restore()


class ArtificialHorizonCard(QWidget):
    """Complete Widget with Horizon Dial, Live Pitch/Roll Digital Badges, and Calibrate Tare Button."""

    def __init__(self, telemetry_client=None, parent=None):
        super().__init__(parent)
        self.telemetry_client = telemetry_client
        self._init_ui()

    def _init_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(12, 12, 12, 12)
        layout.setSpacing(8)

        # Title Row
        title_layout = QHBoxLayout()
        title_lbl = QLabel("🧭 ARTIFICIAL HORIZON")
        title_lbl.setStyleSheet("font-size: 11px; font-weight: 800; color: #00f0ff; letter-spacing: 1px;")
        title_layout.addWidget(title_lbl)
        title_layout.addStretch()

        self.status_badge = QLabel("READY")
        self.status_badge.setStyleSheet(
            "background: rgba(16, 185, 129, 0.15); color: #34d399; "
            "border: 1px solid #059669; border-radius: 8px; padding: 2px 6px; font-size: 10px; font-weight: 800;"
        )
        title_layout.addWidget(self.status_badge)
        layout.addLayout(title_layout)

        # Dial Widget
        self.dial = HorizonInstrument(self)
        layout.addWidget(self.dial, alignment=Qt.AlignmentFlag.AlignCenter)

        # Digital Readout Badges
        metrics_layout = QHBoxLayout()
        metrics_layout.setSpacing(6)

        self.pitch_badge = self._create_metric_box("PITCH", "0.0°")
        self.roll_badge = self._create_metric_box("ROLL", "0.0°")
        metrics_layout.addWidget(self.pitch_badge)
        metrics_layout.addWidget(self.roll_badge)
        layout.addLayout(metrics_layout)

        # Accel & Tare Row
        bottom_layout = QHBoxLayout()
        self.accel_lbl = QLabel("ACC: 0.00, 0.00, 1.00g")
        self.accel_lbl.setStyleSheet("font-size: 10px; color: #94a3b8; font-family: ui-monospace, SFMono-Regular, monospace;")
        bottom_layout.addWidget(self.accel_lbl)
        bottom_layout.addStretch()

        from PySide6.QtWidgets import QPushButton
        self.btn_zero = QPushButton("🎯 Zero IMU")
        self.btn_zero.setStyleSheet(
            "QPushButton { background: #131b2e; border: 1px solid #1e293b; color: #00f0ff; "
            "border-radius: 6px; padding: 3px 8px; font-size: 10px; font-weight: 800; } "
            "QPushButton:hover { background: #1e293b; border-color: #00f0ff; } "
            "QPushButton:pressed { background: #00f0ff; color: #000; }"
        )
        self.btn_zero.clicked.connect(self._on_zero_imu)
        bottom_layout.addWidget(self.btn_zero)
        layout.addLayout(bottom_layout)

        self.setStyleSheet("""
            ArtificialHorizonCard {
                background-color: #0f1523;
                border: 1px solid #1e293b;
                border-radius: 12px;
            }
        """)

    def _create_metric_box(self, label: str, init_val: str):
        box = QWidget()
        box_layout = QVBoxLayout(box)
        box_layout.setContentsMargins(6, 4, 6, 4)
        box_layout.setSpacing(2)
        box.setStyleSheet("background: rgba(0,0,0,0.35); border: 1px solid #1e293b; border-radius: 8px;")

        lbl_title = QLabel(label)
        lbl_title.setStyleSheet("font-size: 9px; font-weight: 700; color: #94a3b8;")
        lbl_val = QLabel(init_val)
        lbl_val.setStyleSheet("font-size: 14px; font-weight: 800; color: #00f0ff; font-family: ui-monospace, monospace;")
        lbl_val.setObjectName(f"val_{label.lower()}")

        box_layout.addWidget(lbl_title, alignment=Qt.AlignmentFlag.AlignCenter)
        box_layout.addWidget(lbl_val, alignment=Qt.AlignmentFlag.AlignCenter)
        return box

    def _on_zero_imu(self):
        if self.telemetry_client:
            self.telemetry_client.send_imu_calibrate()

    def update_telemetry(self, data: dict):
        if not isinstance(data, dict):
            return

        imu = data.get("imu", {})
        ready = imu.get("ready", False)
        pitch = float(imu.get("pitch", 0.0))
        roll = float(imu.get("roll", 0.0))
        ax = float(imu.get("ax", 0.0))
        ay = float(imu.get("ay", 0.0))
        az = float(imu.get("az", 1.0))

        self.dial.update_attitude(pitch, roll, ready)

        val_pitch = self.pitch_badge.findChild(QLabel, "val_pitch")
        val_roll = self.roll_badge.findChild(QLabel, "val_roll")

        if val_pitch:
            val_pitch.setText(f"{'+' if pitch >= 0 else ''}{pitch:.1f}°")
        if val_roll:
            val_roll.setText(f"{'+' if roll >= 0 else ''}{roll:.1f}°")

        self.accel_lbl.setText(f"ACC: {ax:.2f}, {ay:.2f}, {az:.2f}g")

        if ready:
            self.status_badge.setText("READY")
            self.status_badge.setStyleSheet(
                "background: rgba(16, 185, 129, 0.15); color: #34d399; "
                "border: 1px solid #059669; border-radius: 8px; padding: 2px 6px; font-size: 10px; font-weight: 800;"
            )
        else:
            self.status_badge.setText("OFFLINE")
            self.status_badge.setStyleSheet(
                "background: rgba(239, 68, 68, 0.15); color: #f87171; "
                "border: 1px solid #dc2626; border-radius: 8px; padding: 2px 6px; font-size: 10px; font-weight: 800;"
            )
