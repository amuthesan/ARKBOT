#!/usr/bin/env python3
"""
ARK-BOT Desktop Companion GUI Launcher
Run: python3 gui/run.py
"""

import sys
import os
import signal

# Add current directory to path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    from PySide6.QtWidgets import QApplication
    from PySide6.QtCore import Qt
except ImportError:
    print("[ERROR] PySide6 is required. Please install it using: pip install PySide6")
    sys.exit(1)

from app import MainWindow


def main():
    # Handle Ctrl+C gracefully
    signal.signal(signal.SIGINT, signal.SIG_DFL)

    app = QApplication(sys.argv)
    app.setApplicationName("ARK-BOT Companion GUI")
    app.setOrganizationName("Ark Technology")

    window = MainWindow()
    window.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
