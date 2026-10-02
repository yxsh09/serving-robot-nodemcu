# Serving Robot for Restaurant Using NodeMCU

A practical prototype of a restaurant serving robot based on **NodeMCU ESP8266**.

The robot:
- follows a marked line using IR sensors
- receives a table number from a 4x4 keypad
- shows status/order information on an I2C LCD
- uses an MFRC522 RFID reader to identify the destination table
- stops when an obstacle is detected by an HC-SR04 ultrasonic sensor
- drives two DC geared motors through an L298N motor driver
- uses LEDs and a buzzer for status/fault indication

## Project basis

This implementation is based on the supplied seminar report, which specifies NodeMCU, motor driver, wheel motors, IR line sensor, keypad, LCD, RFID reader and battery as the main hardware. The report also describes autonomous line following, table identification, obstacle detection and keypad-based delivery. 

## Hardware

| Component | Quantity | Purpose |
|---|---:|---|
| NodeMCU ESP8266 | 1 | Main controller |
| L298N motor driver | 1 | Drives left/right DC motors |
| DC geared motors | 2 | Robot movement |
| IR line sensors | 3 | Line detection |
| MFRC522 RFID reader | 1 | Table identification |
| RFID tags | 4+ | Table IDs |
| 4x4 membrane keypad | 1 | Select table |
| 16x2 I2C LCD | 1 | Status display |
| HC-SR04 ultrasonic sensor | 1 | Obstacle detection |
| Buzzer | 1 | Obstacle alert |
| Red/Green/Orange LEDs | 1 each | Status |
| LiFePO4 battery + suitable regulator | 1 | Power |
| Robot chassis + caster | 1 | Mechanical platform |

## Pin map

### L298N
- ENA -> D5 (GPIO14) PWM
- IN1 -> D1 (GPIO5)
- IN2 -> D2 (GPIO4)
- IN3 -> D6 (GPIO12)
- IN4 -> D7 (GPIO13)
- ENB -> D8 (GPIO15) PWM

### 3-channel IR sensor
- Left -> D0 (GPIO16)
- Center -> D3 (GPIO0)
- Right -> D4 (GPIO2)

### HC-SR04
- TRIG -> RX (GPIO3)
- ECHO -> TX (GPIO1)

> The HC-SR04 echo can be 5 V on common modules. Use a voltage divider/level shifter before connecting ECHO to the ESP8266.

### MFRC522
- SDA/SS -> SD3 (GPIO10)
- SCK -> SD2 (GPIO9)
- MOSI -> SD1 (GPIO8)
- MISO -> SD0 (GPIO16)
- RST -> A0
- 3.3 V -> 3V3
- GND -> GND

> ESP8266 boot/flash pins are board-dependent. For a physical prototype, verify the exact NodeMCU board pinout and avoid conflicts. An alternative is to move the RFID interface to a stable SPI-capable pin arrangement on your board.

### I2C LCD
- SDA -> D2
- SCL -> D1

### LEDs and buzzer
- Red LED -> A0
- Green LED -> D4
- Orange LED -> D3
- Buzzer -> D8

### Keypad
The 4x4 keypad needs 8 GPIO lines. Because an ESP8266 NodeMCU has limited convenient GPIOs, this repository provides two configurations:
1. `config.h` contains a **default keypad matrix** intended for an I/O expander.
2. For a direct-wired keypad, use a PCF8574/MCP23017 I/O expander and update the keypad pins.

For a classroom prototype, the simplest reliable architecture is:
- PCF8574 -> 4x4 keypad
- ESP8266 -> PCF8574 over I2C

## Software

Arduino IDE / PlatformIO can be used.

Required libraries:
- ESP8266 Arduino Core
- `LiquidCrystal_I2C`
- `Keypad`
- `MFRC522`

## Table logic

The keypad selects:
- `1` -> Table 1
- `2` -> Table 2
- `3` -> Table 3
- `4` -> Table 4

The robot then follows the line. At an RFID checkpoint, it reads the tag and compares the UID with the configured table UID. If the UID matches the selected table, the robot stops and displays `DELIVERED`.

## Build flow

```text
START
  |
  v
Select table using keypad
  |
  v
Confirm table on LCD
  |
  v
Start motors
  |
  v
Follow line with IR sensors
  |
  +---- obstacle? ---- YES ---> STOP + BUZZER ---> wait ---> retry
  |
  NO
  |
  v
Read RFID at checkpoint
  |
  +---- selected table? ---- NO ---> continue
  |
  YES
  |
  v
STOP -> display delivered -> buzzer
  |
  v
Wait for next order
```

## Important practical note

The original report combines several hardware descriptions and mentions both keypad/RFID operation and ultrasonic obstacle stopping. This repository treats the report as the project specification and turns those ideas into a single modular prototype.

For a real restaurant deployment, add emergency stop, battery protection, mechanical guards, safe motor-current limits, controlled acceleration/deceleration, and proper electrical isolation.

## Repository structure

```text
serving-robot-nodemcu/
├── README.md
├── LICENSE
├── .gitignore
├── platformio.ini
├── include/
│   └── config.h
├── src/
│   └── main.cpp
├── docs/
│   ├── WIRING.md
│   ├── FLOWCHART.md
│   ├── TESTING.md
│   └── GITHUB_UPLOAD.md
└── hardware/
    └── BOM.md
```
