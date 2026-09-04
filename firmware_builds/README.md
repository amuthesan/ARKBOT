# ARK-BOT Firmware Releases & Build Binaries

This directory stores compiled release binary files (`.bin`) for each version of **ARK-BOT**.

---

## 📁 Directory Structure

```
firmware_builds/
├── v1.0.5/
│   ├── firmware.bin          # OTA update binary (upload via http://arkbot.local/update)
│   └── firmware.factory.bin  # Factory flashing binary (includes bootloader, partitions, and app)
├── v1.0.4/
├── v1.0.3/
├── v1.0.1/
└── v0.1.1/
```

---

## 🚀 How to Flash

### Method 1: Over-The-Air (OTA) via Web Browser (Recommended)
1. Power on ARK-BOT and connect to the same Wi-Fi network.
2. Open [`http://arkbot.local/update`](http://arkbot.local/update) in your web browser.
3. Drag and drop or browse for `firmware.bin` from the desired version folder.
4. Click **⚡ START OTA FIRMWARE FLASH**.
5. Wait for the upload progress to reach 100%. The robot will automatically restart into the new firmware.

---

### Method 2: USB Cable Flashing (esptool)
```bash
# Flash the complete factory bundle:
esptool.py --chip esp32c6 --port /dev/cu.usbmodem* write_flash 0x0 firmware_builds/v1.0.4/firmware.factory.bin

# Or flash application partition only:
esptool.py --chip esp32c6 --port /dev/cu.usbmodem* write_flash 0x10000 firmware_builds/v1.0.4/firmware.bin
```
