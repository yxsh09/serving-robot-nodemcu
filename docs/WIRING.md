# Wiring Guide

## 1. Power

Use separate regulated power paths where necessary:

```text
Battery
  |
  +---- Motor supply -> L298N -> DC motors
  |
  +---- Regulator -> NodeMCU 5V/VIN
  |
  +---- 3.3V logic -> ESP8266 peripherals that require 3.3V
```

All grounds must share a common reference.

## 2. Motor driver

```text
NodeMCU D5  -> L298N ENA
NodeMCU D1  -> L298N IN1
NodeMCU D2  -> L298N IN2

NodeMCU D6  -> L298N IN3
NodeMCU D7  -> L298N IN4
NodeMCU D8  -> L298N ENB

L298N OUT1/OUT2 -> Left motor
L298N OUT3/OUT4 -> Right motor
```

Remove the L298N ENA/ENB jumpers if PWM speed control is required.

## 3. Line sensors

```text
Left sensor   -> D0
Center sensor -> D3
Right sensor  -> D4
VCC           -> sensor-rated supply
GND           -> common GND
```

The code assumes LOW means the sensor sees the line. Change the logic if your module is inverted.

## 4. Ultrasonic

```text
TRIG -> D9
ECHO -> D10
```

Use a voltage divider or logic-level converter on the ECHO line if the sensor output is above the ESP8266 GPIO voltage limit.

## 5. RFID

The MFRC522 communicates through SPI and should use a stable 3.3 V supply. Keep wires short.

## 6. LCD

```text
SDA -> I2C SDA
SCL -> I2C SCL
VCC -> module-rated supply
GND -> common GND
```

Typical 16x2 I2C backpack address is `0x27`, but scan your module if needed.

## 7. Keypad

A 4x4 keypad requires 8 digital signals. Because the ESP8266 has limited convenient GPIOs, use a PCF8574 or MCP23017 I/O expander.

## 8. RFID table tags

Install one RFID tag at each table/checkpoint:

```text
Kitchen -> line -> RFID checkpoint Table 1
                 -> RFID checkpoint Table 2
                 -> RFID checkpoint Table 3
                 -> RFID checkpoint Table 4
```

The software compares the scanned UID with the UID configured for the selected table.
