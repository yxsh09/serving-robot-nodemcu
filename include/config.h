#pragma once

// ============================================================
// Serving Robot - NodeMCU ESP8266 configuration
// ============================================================

// Motor driver: L298N
#define PIN_ENA D5
#define PIN_IN1 D1
#define PIN_IN2 D2
#define PIN_IN3 D6
#define PIN_IN4 D7
#define PIN_ENB D8

// 3 IR line sensors
#define PIN_IR_LEFT   D0
#define PIN_IR_CENTER D3
#define PIN_IR_RIGHT  D4

// Ultrasonic sensor
#define PIN_TRIG  D9
#define PIN_ECHO D10

// Status indicators
#define PIN_RED_LED    A0
#define PIN_GREEN_LED  D4
#define PIN_ORANGE_LED D3
#define PIN_BUZZER    D8

// I2C LCD
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Motor speeds (0-255)
#define BASE_SPEED 150
#define TURN_SPEED 115
#define MAX_SPEED 200

// Obstacle distance in cm
#define OBSTACLE_DISTANCE_CM 25

// RFID UID examples.
// Replace these with the UID strings printed by your own RFID tags.
#define TABLE1_UID "DE AD BE EF"
#define TABLE2_UID "11 22 33 44"
#define TABLE3_UID "12 34 56 78"
#define TABLE4_UID "AA BB CC DD"

// Keypad example using an external PCF8574-style matrix interface.
// If using the Keypad library directly, update these pins for your
// actual hardware and avoid GPIO boot/flash conflicts.
