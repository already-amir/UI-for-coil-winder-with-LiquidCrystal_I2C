# UI for Coil Winder with LiquidCrystal_I2C

A user interface system for a **Coil Winder machine**, built on **ESP32 Devkit V1** using a **16x2 I2C LCD display**.  
This project focuses on creating an interactive and intuitive **graphical environment** for coil winding machines — the motor control system will be integrated in future updates.

---

## 🧠 Overview

This project provides a complete **menu-driven interface** for configuring coil winding parameters such as:
- Number of turns  
- Coil width  
- Wire diameter  
- EEPROM-based save and load system  

It allows users to easily navigate settings, store multiple presets, and view system information through a clean LCD interface.

---

## ⚙️ Features

- 🧭 **Menu Navigation System** (multi-level, intuitive)
- 💾 **EEPROM Save/Load** (up to 25 slots)
- 🔄 **Real-time Winding Status** display
- 🧰 **Adjustable Parameters**:
  - Number of turns
  - Coil width
  - Wire diameter
- 🧱 **State Zero Mode** (manual positioning placeholder)
- 🧩 Modular structure for future integration with motor drivers

---

## 🪛 Hardware Setup

**Microcontroller:** ESP32 Devkit V1  
**Display:** 16x2 I2C LCD (address: `0x27`)  
**Buttons:**
| Function | GPIO Pin |
|-----------|-----------|
| Up        | 12 |
| Down      | 13 |
| OK        | 27 |
| Back      | 14 |

> ⚠️ Ensure proper pull-up configuration (all buttons use `INPUT_PULLUP`).

---

## 🔌 Wiring Diagram (Basic)

| Component | ESP32 Pin | Description |
|------------|------------|-------------|
| LCD SDA | 21 | I2C Data |
| LCD SCL | 22 | I2C Clock |
| UP Button | 12 | Pull-up input |
| DOWN Button | 13 | Pull-up input |
| OK Button | 27 | Pull-up input |
| BACK Button | 14 | Pull-up input |

> Motor driver and coil control pins will be added in future updates.

---

## 🧩 Software Structure

```text
main.ino
│
├── UI System (LCD rendering)
│   ├── show() → Display menu & data
│   ├── Navigation → up_key(), down_key()
│
├── EEPROM Storage
│   ├── save_settings(index)
│   ├── load_all_settings()
│   └── zero_saves()
│
└── Menu States
    ├── Main Menu
    ├── Settings
    ├── Load / Save
    ├── Info
    └── Zero State

```

## 🚀 Getting Started
 🧩 Prerequisites

Install the following libraries in your Arduino IDE:

- LiquidCrystal_I2C
-  EEPROM
-  Wire



## 🧠 Uploading the Code

- Open main.ino in Arduino IDE.

- Select Board: ESP32 Devkit V1

- Connect the board via USB.

- Click Upload.

The LCD should initialize with the main menu.

## 🧰 Future Improvements

Integration with stepper motor drivers (A4988 / DRV8825 / etc.)

Add auto calibration (zero state)

Add speed control & winding progress tracking

Support for larger displays (20x4 or OLED)

## 🖼️ Project Photos

<p align="center">
  <img src="pic\1.jpg" width="50% "  style="transform: rotate(-90deg);>
</p>




## 🪪 License

This project is licensed under the MIT License — feel free to use, modify, and distribute with proper credit.