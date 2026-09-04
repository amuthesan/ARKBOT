#!/usr/bin/env python3
"""
ARK-BOT Desktop Companion GUI
Main Application Window with Cyber-Dark Theme, 3D Kinematics Visualizer,
Locomotion Commander, Servo Calibrator, and Real-time Telemetry.
"""

import sys
import os
from PySide6.QtWidgets import (
    QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QSplitter, QTabWidget, QComboBox, QLineEdit, QPushButton,
    QLabel, QStatusBar, QFrame, QMessageBox
)
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QFont, QColor, QIcon, QPixmap


from telemetry_client import TelemetryWorker, ConnectionState, list_serial_ports
from viewport_3d import Viewport3D
from panels.commander_panel import CommanderPanel
from panels.calibrator_panel import CalibratorPanel
from panels.telemetry_panel import TelemetryPanel

CYBER_QSS = """
QMainWindow, QWidget {
    background-color: #080c14;
    color: #e2e8f0;
    font-family: "SF Pro Text", "Helvetica Neue", Arial, sans-serif;
    font-size: 13px;
}

QGroupBox {
    background-color: #0f172a;
    border: 1px solid #1e293b;
    border-radius: 8px;
    margin-top: 14px;
    padding: 12px;
    font-weight: bold;
    color: #38bdf8;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 0 6px;
    background-color: #0f172a;
}

QPushButton {
    background-color: #1e293b;
    color: #f8fafc;
    border: 1px solid #334155;
    border-radius: 6px;
    padding: 8px 14px;
    font-weight: 600;
}
QPushButton:hover {
    background-color: #334155;
    border-color: #00f0ff;
    color: #00f0ff;
}
QPushButton:pressed {
    background-color: #0f172a;
}

QPushButton#btn_stand {
    background-color: #064e3b;
    border-color: #059669;
    color: #34d399;
}
QPushButton#btn_stand:hover {
    background-color: #059669;
    color: #ffffff;
}

QPushButton#btn_sit {
    background-color: #1e1b4b;
    border-color: #6366f1;
    color: #a5b4fc;
}
QPushButton#btn_sit:hover {
    background-color: #4f46e5;
    color: #ffffff;
}

QPushButton#btn_stop {
    background-color: #7f1d1d;
    border-color: #dc2626;
    color: #fca5a5;
    font-weight: bold;
}
QPushButton#btn_stop:hover {
    background-color: #dc2626;
    color: #ffffff;
}

QPushButton#btn_fwd, QPushButton#btn_left, QPushButton#btn_right, QPushButton#btn_back {
    background-color: #0f172a;
    border: 1px solid #0284c7;
    color: #38bdf8;
    font-size: 14px;
}
QPushButton#btn_fwd:hover, QPushButton#btn_left:hover, QPushButton#btn_right:hover, QPushButton#btn_back:hover {
    background-color: #0284c7;
    color: #ffffff;
}

QPushButton#btn_center_all {
    background-color: #3b0764;
    border-color: #a855f7;
    color: #e9d5ff;
}
QPushButton#btn_center_all:hover {
    background-color: #9333ea;
    color: #ffffff;
}

QPushButton#btn_pwr_on {
    background-color: #064e3b;
    border-color: #10b981;
    color: #6ee7b7;
}
QPushButton#btn_pwr_off {
    background-color: #450a0a;
    border-color: #ef4444;
    color: #fca5a5;
}

QTabWidget::pane {
    border: 1px solid #1e293b;
    background-color: #0f172a;
    border-radius: 8px;
}
QTabBar::tab {
    background-color: #080c14;
    color: #94a3b8;
    border: 1px solid #1e293b;
    border-bottom: none;
    padding: 8px 16px;
    margin-right: 4px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    font-weight: 600;
}
QTabBar::tab:selected {
    background-color: #0f172a;
    color: #00f0ff;
    border-top: 2px solid #00f0ff;
}

QSlider::groove:horizontal {
    border: 1px solid #334155;
    height: 6px;
    background: #1e293b;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: #00f0ff;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    border: 2px solid #00f0ff;
    width: 16px;
    margin-top: -6px;
    margin-bottom: -6px;
    border-radius: 8px;
}

QComboBox, QLineEdit, QSpinBox {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 6px;
    padding: 6px 10px;
    color: #f8fafc;
}
QComboBox:focus, QLineEdit:focus, QSpinBox:focus {
    border-color: #00f0ff;
}

QTableWidget {
    background-color: #070a0f;
    border: 1px solid #1e293b;
    gridline-color: #1e293b;
    color: #f8fafc;
}
QHeaderView::section {
    background-color: #0f172a;
    color: #38bdf8;
    padding: 6px;
    border: 1px solid #1e293b;
    font-weight: bold;
}
"""


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ARK-BOT // Desktop Companion & Kinematics Commander")
        self.resize(1280, 820)
        self.setStyleSheet(CYBER_QSS)

        # Set Window Icon using product logo
        logo_icon_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "logo", "logo_white_256.png")
        if not os.path.exists(logo_icon_path):
            logo_icon_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "logo", "White.png")
        if os.path.exists(logo_icon_path):
            self.setWindowIcon(QIcon(logo_icon_path))

        # Telemetry Worker Thread
        self.worker = TelemetryWorker(self)
        self.worker.state_changed.connect(self._on_state_changed)
        self.worker.telemetry_ready.connect(self._on_telemetry_ready)
        self.worker.log_event.connect(self._on_log_event)

        self.init_ui()
        self._refresh_serial_ports()

        # 60 FPS LERP Animation Timer for Viewport
        self.anim_timer = QTimer(self)
        self.anim_timer.timeout.connect(self.viewport.advance_animation)
        self.anim_timer.start(16)  # ~60 FPS

        # Periodic Port Watcher (every 3 seconds when disconnected)
        self.port_scan_timer = QTimer(self)
        self.port_scan_timer.timeout.connect(self._auto_scan_ports)
        self.port_scan_timer.start(3000)

    def init_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        root_layout = QVBoxLayout(central)
        root_layout.setContentsMargins(12, 12, 12, 12)
        root_layout.setSpacing(10)

        # 1. Top Connection Header Bar
        header_bar = self._create_header_bar()
        root_layout.addWidget(header_bar)

        # 2. Main Workspace Splitter
        splitter = QSplitter(Qt.Horizontal)

        # Left: 3D Kinematics Viewport
        viewport_container = QWidget()
        vp_layout = QVBoxLayout(viewport_container)
        vp_layout.setContentsMargins(0, 0, 0, 0)
        self.viewport = Viewport3D()
        vp_layout.addWidget(self.viewport)
        splitter.addWidget(viewport_container)

        # Right: Tabbed Panels
        self.tabs = QTabWidget()
        self.commander_panel = CommanderPanel()
        self.commander_panel.action_requested.connect(self.worker.send_action)
        self.commander_panel.height_requested.connect(self.worker.send_walk_height)

        self.calibrator_panel = CalibratorPanel()
        self.calibrator_panel.servo_changed.connect(self.worker.send_servo)
        self.calibrator_panel.power_toggled.connect(self.worker.send_power)
        self.calibrator_panel.init_requested.connect(self.worker.send_init)

        self.telemetry_panel = TelemetryPanel()

        self.tabs.addTab(self.commander_panel, "🎮 Locomotion Commander")
        self.tabs.addTab(self.calibrator_panel, "🎯 Joint Calibrator")
        self.tabs.addTab(self.telemetry_panel, "📊 Diagnostics & Telemetry")

        splitter.addWidget(self.tabs)
        splitter.setStretchFactor(0, 3)
        splitter.setStretchFactor(1, 2)
        root_layout.addWidget(splitter)

        # 3. Status Bar
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)
        self.status_bar.showMessage("Ready. Connect via USB Serial or enter robot Wi-Fi IP.")

    def _create_header_bar(self):
        frame = QFrame()
        frame.setStyleSheet("background-color: #0f172a; border: 1px solid #1e293b; border-radius: 8px; padding: 4px;")
        layout = QHBoxLayout(frame)
        layout.setContentsMargins(8, 6, 8, 6)
        layout.setSpacing(12)

        # Brand Container (Product Logo + Text)
        brand_widget = QWidget()
        brand_widget.setStyleSheet("background: transparent; border: none;")
        brand_layout = QHBoxLayout(brand_widget)
        brand_layout.setContentsMargins(0, 0, 0, 0)
        brand_layout.setSpacing(8)

        # Product Logo Icon (Vector / Crisp PNG)
        logo_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "logo", "logo_white_128.png")
        if not os.path.exists(logo_path):
            logo_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "logo", "White.png")

        if os.path.exists(logo_path):
            lbl_logo = QLabel()
            lbl_logo.setStyleSheet("background: transparent; border: none;")
            pix = QPixmap(logo_path).scaled(26, 26, Qt.KeepAspectRatio, Qt.SmoothTransformation)
            lbl_logo.setPixmap(pix)
            brand_layout.addWidget(lbl_logo)

        lbl_title = QLabel("ARK-BOT")
        lbl_title.setStyleSheet("font-size: 16px; font-weight: 800; color: #00f0ff; letter-spacing: 1.5px; background: transparent; border: none;")
        brand_layout.addWidget(lbl_title)
        layout.addWidget(brand_widget)

        # Mode Selector
        self.combo_mode = QComboBox()
        self.combo_mode.addItems(["USB Serial", "Wi-Fi (HTTP)"])
        self.combo_mode.currentIndexChanged.connect(self._on_mode_changed)
        layout.addWidget(self.combo_mode)

        # Serial Port Box
        self.serial_widget = QWidget()
        serial_layout = QHBoxLayout(self.serial_widget)
        serial_layout.setContentsMargins(0, 0, 0, 0)
        serial_layout.setSpacing(6)

        self.combo_ports = QComboBox()
        self.combo_ports.setMinimumWidth(260)
        self.btn_refresh = QPushButton("🔄 Refresh")
        self.btn_refresh.setToolTip("Rescan Serial Ports")
        self.btn_refresh.clicked.connect(self._refresh_serial_ports)

        serial_layout.addWidget(self.combo_ports)
        serial_layout.addWidget(self.btn_refresh)
        layout.addWidget(self.serial_widget)

        # Wi-Fi Box
        self.wifi_widget = QWidget()
        wifi_layout = QHBoxLayout(self.wifi_widget)
        wifi_layout.setContentsMargins(0, 0, 0, 0)
        wifi_layout.setSpacing(6)

        self.txt_ip = QLineEdit("arkbot.local")
        self.txt_ip.setPlaceholderText("IP or arkbot.local")
        self.txt_ip.setMinimumWidth(180)
        wifi_layout.addWidget(self.txt_ip)
        layout.addWidget(self.wifi_widget)
        self.wifi_widget.setVisible(False)

        # Connect Button
        self.btn_connect = QPushButton("⚡ CONNECT")
        self.btn_connect.setStyleSheet("background-color: #0284c7; color: #ffffff; font-weight: bold;")
        self.btn_connect.clicked.connect(self._toggle_connection)
        layout.addWidget(self.btn_connect)

        layout.addStretch()

        # Connection Status Pill
        self.lbl_status_pill = QLabel("DISCONNECTED")
        self.lbl_status_pill.setStyleSheet(
            "background-color: #334155; color: #94a3b8; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
        )
        layout.addWidget(self.lbl_status_pill)

        return frame

    def _on_mode_changed(self, index):
        if index == 0:  # Serial
            self.serial_widget.setVisible(True)
            self.wifi_widget.setVisible(False)
        else:  # Wi-Fi
            self.serial_widget.setVisible(False)
            self.wifi_widget.setVisible(True)

    def _refresh_serial_ports(self):
        current_selection = self.combo_ports.currentData()
        self.combo_ports.clear()
        ports = list_serial_ports()
        found_current = False
        default_index = 0

        for idx, (device, desc) in enumerate(ports):
            self.combo_ports.addItem(desc, device)
            if device == current_selection:
                default_index = idx
                found_current = True
            elif not found_current and ("usbmodem" in device or "COM" in device):
                default_index = idx

        if not ports:
            self.combo_ports.addItem("No serial ports found", "")
        else:
            self.combo_ports.setCurrentIndex(default_index)

    def _auto_scan_ports(self):
        if not self.worker.isRunning() and self.combo_mode.currentIndex() == 0:
            # Silently refresh port list if changed
            ports = list_serial_ports()
            if len(ports) != self.combo_ports.count():
                self._refresh_serial_ports()

    def _toggle_connection(self):
        if self.worker.isRunning():
            self.worker.disconnect_device()
        else:
            mode_idx = self.combo_mode.currentIndex()
            if mode_idx == 0:  # Serial
                port = self.combo_ports.currentData()
                if not port:
                    QMessageBox.warning(self, "No Port Selected", "Please select a valid USB Serial port.")
                    return
                self.btn_connect.setText("CONNECTING...")
                self.worker.connect_serial(port)
            else:  # Wi-Fi
                host = self.txt_ip.text().strip()
                if not host:
                    QMessageBox.warning(self, "No Host Entered", "Please enter robot IP or arkbot.local.")
                    return
                self.btn_connect.setText("CONNECTING...")
                self.worker.connect_wifi(host)

    def _on_state_changed(self, state, msg):
        self.status_bar.showMessage(msg)

        if state == ConnectionState.STREAMING:
            self.btn_connect.setText("DISCONNECT")
            self.btn_connect.setStyleSheet("background-color: #7f1d1d; color: #ffffff; font-weight: bold;")
            self.lbl_status_pill.setText("ONLINE (STREAMING)")
            self.lbl_status_pill.setStyleSheet(
                "background-color: #065f46; color: #34d399; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
            )
        elif state == ConnectionState.PORT_OPEN:
            self.btn_connect.setText("DISCONNECT")
            self.btn_connect.setStyleSheet("background-color: #7f1d1d; color: #ffffff; font-weight: bold;")
            self.lbl_status_pill.setText("PORT OPEN (WAITING DATA)")
            self.lbl_status_pill.setStyleSheet(
                "background-color: #854d0e; color: #fef08a; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
            )
        elif state == ConnectionState.CONNECTING:
            self.btn_connect.setText("CONNECTING...")
            self.btn_connect.setStyleSheet("background-color: #1e3a8a; color: #93c5fd; font-weight: bold;")
            self.lbl_status_pill.setText("CONNECTING...")
            self.lbl_status_pill.setStyleSheet(
                "background-color: #1e3a8a; color: #93c5fd; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
            )
        elif state == ConnectionState.ERROR:
            self.btn_connect.setText("⚡ CONNECT")
            self.btn_connect.setStyleSheet("background-color: #0284c7; color: #ffffff; font-weight: bold;")
            self.lbl_status_pill.setText("ERROR")
            self.lbl_status_pill.setStyleSheet(
                "background-color: #7f1d1d; color: #fca5a5; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
            )
        else:  # DISCONNECTED
            self.btn_connect.setText("⚡ CONNECT")
            self.btn_connect.setStyleSheet("background-color: #0284c7; color: #ffffff; font-weight: bold;")
            self.lbl_status_pill.setText("DISCONNECTED")
            self.lbl_status_pill.setStyleSheet(
                "background-color: #334155; color: #94a3b8; padding: 5px 12px; border-radius: 12px; font-weight: bold; font-size: 11px;"
            )

    def _on_telemetry_ready(self, telem):
        # 1. Update 3D viewport kinematics target
        self.viewport.update_telemetry(telem)

        # 2. Sync walk height if provided
        if "walk_height" in telem:
            self.commander_panel.update_walk_height(telem["walk_height"])

        # 3. Update panel data
        current_tab_idx = self.tabs.currentIndex()
        if current_tab_idx == 1:  # Calibrator tab active
            self.calibrator_panel.update_telemetry(telem)
        elif current_tab_idx == 2:  # Diagnostics tab active
            self.telemetry_panel.update_telemetry(telem)
        else:
            # Background sync for other tabs
            self._bg_counter = getattr(self, '_bg_counter', 0) + 1
            if self._bg_counter % 8 == 0:
                self.calibrator_panel.update_telemetry(telem)
                self.telemetry_panel.update_telemetry(telem)

    def _on_log_event(self, level, msg):
        self.telemetry_panel.append_log(level, msg)

    def closeEvent(self, event):
        self.worker.disconnect_device()
        event.accept()
