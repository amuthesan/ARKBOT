#!/usr/bin/env python3
"""
ARK-BOT Companion GUI - Ultra-Fast Telemetry Client
Direct IP resolution, non-blocking Serial & Wi-Fi communication engine.
"""

import json
import time
import queue
import socket
import urllib.request
import urllib.parse
from PySide6.QtCore import QThread, Signal, QMutex, QMutexLocker
import serial
import serial.tools.list_ports


def list_serial_ports():
    """Returns available serial ports prioritizing ESP32 / USB Modem devices."""
    ports = serial.tools.list_ports.comports()
    result = []
    for p in ports:
        desc = p.description if p.description else "Serial Device"
        result.append((p.device, f"{p.device} ({desc})"))
    result.sort(key=lambda x: (not ("usbmodem" in x[0] or "USB" in x[1] or "COM" in x[0]), x[0]))
    return result


class ConnectionState:
    DISCONNECTED = "DISCONNECTED"
    CONNECTING = "CONNECTING"
    PORT_OPEN = "PORT OPEN (WAITING DATA)"
    STREAMING = "ONLINE (STREAMING)"
    ERROR = "ERROR"


class TelemetryWorker(QThread):
    state_changed = Signal(str, str)        # (state, message)
    telemetry_ready = Signal(dict)          # (telemetry_dict)
    log_event = Signal(str, str)            # (level, message)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.mode = "wifi"
        self.serial_port = ""
        self.serial_baud = 115200
        self.wifi_ip = "arkbot.local"
        self.resolved_ip = "192.168.1.2"
        self.wifi_poll_interval = 0.05  # 20 Hz

        self._running = False
        self._mutex = QMutex()
        self._cmd_queue = queue.Queue()
        self._ser = None
        self._latest_telemetry = None
        self._last_packet_time = 0.0
        self._packet_count = 0

    def connect_serial(self, port, baud=115200):
        self.disconnect_device()
        self.mode = "serial"
        self.serial_port = port
        self.serial_baud = baud
        self._running = True
        self.start()

    def connect_wifi(self, host_or_ip):
        self.disconnect_device()
        self.mode = "wifi"
        host = host_or_ip.strip().replace("http://", "").replace("/", "")
        self.wifi_ip = host if host else "arkbot.local"
        self._running = True
        self.start()

    def disconnect_device(self):
        self._running = False
        if self.isRunning():
            self.quit()
            self.wait(500)
        if self._ser and self._ser.is_open:
            try:
                self._ser.close()
            except Exception:
                pass
            self._ser = None
        self.state_changed.emit(ConnectionState.DISCONNECTED, "Disconnected")

    def get_latest_telemetry(self):
        with QMutexLocker(self._mutex):
            return self._latest_telemetry

    def send_command(self, cmd_dict):
        self._cmd_queue.put(cmd_dict)

    def send_action(self, action_name, steps=1, speed=1.0):
        self.send_command({
            "action": action_name,
            "steps": int(steps),
            "speed": float(speed)
        })

    def send_servo(self, leg, joint, angle):
        self.send_command({
            "servo": {
                "leg": int(leg),
                "joint": int(joint),
                "angle": int(angle)
            }
        })

    def send_power(self, state=True):
        self.send_command({
            "power": {
                "target": "all",
                "state": 1 if state else 0
            }
        })

    def send_init(self, target="all"):
        self.send_command({"init": target})

    def _normalize_telemetry(self, data):
        """Ensures all standard keys are present for both Serial and WebUI formats."""
        if not isinstance(data, dict):
            return data
        
        # PCA State
        if "pca" not in data and "pcaReady" in data:
            data["pca"] = data["pcaReady"]
        if "pcaReady" not in data and "pca" in data:
            data["pcaReady"] = data["pca"]

        # Power State
        if "pwr" not in data and "enabled" in data:
            data["pwr"] = data["enabled"]
        if "enabled" not in data and "pwr" in data:
            data["enabled"] = data["pwr"]

        return data

    def run(self):
        if self.mode == "serial":
            self._run_serial()
        elif self.mode == "wifi":
            self._run_wifi()

    def _run_serial(self):
        self.state_changed.emit(ConnectionState.CONNECTING, f"Opening {self.serial_port}...")
        try:
            self._ser = serial.Serial(
                port=self.serial_port,
                baudrate=self.serial_baud,
                timeout=0.005,
                write_timeout=0.1,
                dsrdtr=False,
                rtscts=False
            )
            try:
                self._ser.dtr = True
                self._ser.rts = False
            except Exception:
                pass

            time.sleep(0.05)
            self._ser.write(b'{"stream":true}\n')
            self._ser.flush()
            self.state_changed.emit(ConnectionState.PORT_OPEN, f"Port {self.serial_port} open. Waiting for telemetry...")
            self.log_event.emit("INFO", f"Serial port opened on {self.serial_port}")
        except Exception as e:
            self.state_changed.emit(ConnectionState.ERROR, f"Serial open failed: {e}")
            self.log_event.emit("ERROR", f"Failed to open {self.serial_port}: {e}")
            self._running = False
            return

        buffer = ""
        last_emit_time = 0.0

        while self._running:
            now = time.time()

            # 1. Outgoing commands
            while not self._cmd_queue.empty():
                try:
                    cmd = self._cmd_queue.get_nowait()
                    payload = (json.dumps(cmd) if isinstance(cmd, dict) else str(cmd).strip()) + "\n"
                    self._ser.write(payload.encode('utf-8'))
                    self._ser.flush()
                    self.log_event.emit("TX", payload.strip())
                except Exception as e:
                    self.log_event.emit("ERROR", f"Serial TX error: {e}")

            # 2. Read stream
            try:
                available = self._ser.in_waiting
                if available > 0:
                    raw = self._ser.read(available).decode('utf-8', errors='ignore')
                    buffer += raw

                    while "\n" in buffer:
                        line, buffer = buffer.split("\n", 1)
                        line = line.strip()
                        if not line:
                            continue

                        if line.startswith("{") and line.endswith("}"):
                            try:
                                data = json.loads(line)
                                data = self._normalize_telemetry(data)
                                with QMutexLocker(self._mutex):
                                    self._latest_telemetry = data
                                    self._last_packet_time = now
                                    self._packet_count += 1

                                if now - last_emit_time >= 0.033:
                                    last_emit_time = now
                                    self.telemetry_ready.emit(data)

                                self.state_changed.emit(
                                    ConnectionState.STREAMING,
                                    f"Streaming @ {self.serial_port} (Pkts: {self._packet_count})"
                                )
                            except Exception:
                                self.log_event.emit("RX_MSG", line)
                        else:
                            if "[CMD]" in line or "[CALIB]" in line or "[OK]" in line or "[ACTION]" in line:
                                self.log_event.emit("RX_MSG", line)
                else:
                    if self._last_packet_time > 0 and (now - self._last_packet_time > 2.0):
                        self.state_changed.emit(
                            ConnectionState.PORT_OPEN,
                            f"Port open, telemetry idle ({now - self._last_packet_time:.1f}s)"
                        )
                    time.sleep(0.002)
            except Exception as e:
                self.state_changed.emit(ConnectionState.ERROR, f"Serial error: {e}")
                self.log_event.emit("ERROR", f"Serial error: {e}")
                break

        if self._ser and self._ser.is_open:
            try:
                self._ser.close()
            except Exception:
                pass
            self._ser = None

    def _run_wifi(self):
        self.state_changed.emit(ConnectionState.CONNECTING, f"Resolving {self.wifi_ip}...")

        # Resolve mDNS host once to prevent 5-second DNS timeouts per socket
        try:
            self.resolved_ip = socket.gethostbyname(self.wifi_ip)
            self.log_event.emit("INFO", f"Resolved {self.wifi_ip} -> {self.resolved_ip}")
        except Exception as e:
            self.resolved_ip = self.wifi_ip

        base_url = f"http://{self.resolved_ip}"
        self.state_changed.emit(ConnectionState.CONNECTING, f"Connecting to {base_url}...")

        # Verify endpoint
        try:
            req = urllib.request.Request(f"{base_url}/api/status", headers={"User-Agent": "ARK-BOT-GUI/1.0"})
            with urllib.request.urlopen(req, timeout=1.0) as resp:
                data = json.loads(resp.read().decode('utf-8'))
                data = self._normalize_telemetry(data)
                with QMutexLocker(self._mutex):
                    self._latest_telemetry = data
                    self._last_packet_time = time.time()
                self.telemetry_ready.emit(data)
                self.state_changed.emit(ConnectionState.STREAMING, f"Online via Wi-Fi ({self.wifi_ip} / {self.resolved_ip})")
                self.log_event.emit("INFO", f"Wi-Fi connected to {self.wifi_ip}")
        except Exception as e:
            self.state_changed.emit(ConnectionState.ERROR, f"Wi-Fi connect failed: {e}")
            self.log_event.emit("ERROR", f"Wi-Fi connect failed: {e}")
            self._running = False
            return

        last_poll = 0
        last_emit_time = 0.0

        while self._running:
            now = time.time()

            # 1. Instant Outgoing Command Dispatch
            while not self._cmd_queue.empty():
                try:
                    cmd = self._cmd_queue.get_nowait()
                    if isinstance(cmd, dict):
                        if "action" in cmd:
                            url = f"{base_url}/api/action"
                            post_data = urllib.parse.urlencode({
                                "action": cmd.get("action", ""),
                                "steps": cmd.get("steps", 1),
                                "speed": cmd.get("speed", 1.0)
                            }).encode('utf-8')
                        elif "servo" in cmd:
                            url = f"{base_url}/api/set"
                            post_data = urllib.parse.urlencode({
                                "leg": cmd["servo"].get("leg", 0),
                                "joint": cmd["servo"].get("joint", 0),
                                "angle": cmd["servo"].get("angle", 90)
                            }).encode('utf-8')
                        elif "power" in cmd:
                            url = f"{base_url}/api/power"
                            post_data = urllib.parse.urlencode({"state": cmd["power"].get("state", 1)}).encode('utf-8')
                        elif "init" in cmd:
                            url = f"{base_url}/api/init"
                            post_data = urllib.parse.urlencode({"target": "all"}).encode('utf-8')
                        else:
                            url = f"{base_url}/api/status"
                            post_data = None

                        req = urllib.request.Request(url, data=post_data, headers={"User-Agent": "ARK-BOT-GUI/1.0"})
                        with urllib.request.urlopen(req, timeout=0.8) as resp:
                            res_text = resp.read().decode('utf-8')
                            try:
                                data = json.loads(res_text)
                                data = self._normalize_telemetry(data)
                                with QMutexLocker(self._mutex):
                                    self._latest_telemetry = data
                                self.telemetry_ready.emit(data)
                            except Exception:
                                pass
                        self.log_event.emit("TX", f"POST {url}")
                except Exception as e:
                    self.log_event.emit("ERROR", f"Wi-Fi command error: {e}")

            # 2. Telemetry Polling (20 Hz)
            if now - last_poll >= self.wifi_poll_interval:
                last_poll = now
                try:
                    req = urllib.request.Request(f"{base_url}/api/status", headers={"User-Agent": "ARK-BOT-GUI/1.0"})
                    with urllib.request.urlopen(req, timeout=0.6) as resp:
                        data = json.loads(resp.read().decode('utf-8'))
                        data = self._normalize_telemetry(data)
                        with QMutexLocker(self._mutex):
                            self._latest_telemetry = data
                            self._last_packet_time = now
                            self._packet_count += 1

                        if now - last_emit_time >= 0.033:
                            last_emit_time = now
                            self.telemetry_ready.emit(data)

                        self.state_changed.emit(ConnectionState.STREAMING, f"Online via Wi-Fi (Pkts: {self._packet_count})")
                except Exception as e:
                    self.state_changed.emit(ConnectionState.PORT_OPEN, f"Wi-Fi poll timeout: {e}")

            time.sleep(0.005)
