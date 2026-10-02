#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include "config.h"

// ------------------------------------------------------------
// LCD
// ------------------------------------------------------------
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// ------------------------------------------------------------
// RFID
// ------------------------------------------------------------
// IMPORTANT:
// SS/RST pins must be adapted to your exact NodeMCU wiring.
// This example keeps them explicit so they are easy to change.
#ifndef RFID_SS
#define RFID_SS D8
#endif

#ifndef RFID_RST
#define RFID_RST D0
#endif

MFRC522 rfid(RFID_SS, RFID_RST);

// ------------------------------------------------------------
// State
// ------------------------------------------------------------
enum RobotState {
  IDLE,
  DELIVERING,
  OBSTACLE_STOP,
  DELIVERED
};

RobotState state = IDLE;
int selectedTable = 0;

// ------------------------------------------------------------
// Motor control
// ------------------------------------------------------------
void setMotor(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // Left motor
  if (leftSpeed >= 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
  }

  // Right motor
  if (rightSpeed >= 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  }

  analogWrite(PIN_ENA, abs(leftSpeed));
  analogWrite(PIN_ENB, abs(rightSpeed));
}

void stopMotors() {
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

// ------------------------------------------------------------
// LCD helpers
// ------------------------------------------------------------
void lcdMessage(const String &line1, const String &line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, LCD_COLS));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, LCD_COLS));
}

// ------------------------------------------------------------
// Buzzer
// ------------------------------------------------------------
void beep(int durationMs = 120) {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(durationMs);
  digitalWrite(PIN_BUZZER, LOW);
}

// ------------------------------------------------------------
// Ultrasonic
// ------------------------------------------------------------
long readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 25000UL);
  if (duration == 0) return 999;

  return (long)(duration * 0.0343 / 2.0);
}

// ------------------------------------------------------------
// Line following
//
// Sensor assumption:
// LOW  = dark/line
// HIGH = floor
//
// If your sensor works opposite, invert these conditions.
// ------------------------------------------------------------
void followLine() {
  int left = digitalRead(PIN_IR_LEFT);
  int center = digitalRead(PIN_IR_CENTER);
  int right = digitalRead(PIN_IR_RIGHT);

  if (left == LOW && center == HIGH && right == LOW) {
    // centered
    setMotor(BASE_SPEED, BASE_SPEED);
  } else if (left == LOW && center == LOW && right == HIGH) {
    // line is toward left
    setMotor(TURN_SPEED, BASE_SPEED);
  } else if (left == HIGH && center == LOW && right == LOW) {
    // line is toward right
    setMotor(BASE_SPEED, TURN_SPEED);
  } else if (left == LOW && center == HIGH && right == HIGH) {
    setMotor(TURN_SPEED, BASE_SPEED);
  } else if (left == HIGH && center == HIGH && right == LOW) {
    setMotor(BASE_SPEED, TURN_SPEED);
  } else if (left == HIGH && center == LOW && right == HIGH) {
    setMotor(TURN_SPEED, TURN_SPEED);
  } else {
    // Unknown/off-line state: stop rather than accelerate blindly.
    stopMotors();
  }
}

// ------------------------------------------------------------
// RFID UID helper
// ------------------------------------------------------------
String uidToString(MFRC522::Uid *uid) {
  String result;
  for (byte i = 0; i < uid->size; i++) {
    if (uid->uidByte[i] < 0x10) result += "0";
    result += String(uid->uidByte[i], HEX);
    if (i < uid->size - 1) result += " ";
  }
  result.toUpperCase();
  return result;
}

String tableUID(int table) {
  switch (table) {
    case 1: return TABLE1_UID;
    case 2: return TABLE2_UID;
    case 3: return TABLE3_UID;
    case 4: return TABLE4_UID;
    default: return "";
  }
}

bool isSelectedTableTag() {
  if (!rfid.PICC_IsNewCardPresent()) return false;
  if (!rfid.PICC_ReadCardSerial()) return false;

  String currentUID = uidToString(&rfid.uid);
  String expectedUID = tableUID(selectedTable);

  Serial.print("RFID UID: ");
  Serial.println(currentUID);

  bool match = currentUID == expectedUID;

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  return match;
}

// ------------------------------------------------------------
// Keypad input placeholder
// ------------------------------------------------------------
//
// A direct 4x4 keypad consumes 8 GPIOs. On ESP8266, this can
// conflict with the other peripherals. For a physical build,
// connect the keypad through a PCF8574/MCP23017 I/O expander.
//
// Replace readKeypad() with your selected expander implementation.
// The demo below accepts commands from Serial so the firmware can
// still be tested without a keypad.
// ------------------------------------------------------------
char readKeypad() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '1' && c <= '4') return c;
    if (c == 's' || c == 'S') return 'S';
  }
  return 0;
}

// ------------------------------------------------------------
// Order selection
// ------------------------------------------------------------
void checkForOrder() {
  char key = readKeypad();

  if (key >= '1' && key <= '4') {
    selectedTable = key - '0';
    state = DELIVERING;

    digitalWrite(PIN_ORANGE_LED, HIGH);
    digitalWrite(PIN_GREEN_LED, HIGH);

    lcdMessage(
      "Table selected:",
      "Table " + String(selectedTable)
    );

    beep();
    delay(600);
    lcdMessage("Delivering...", "Follow line");
  }
}

// ------------------------------------------------------------
// Obstacle handling
// ------------------------------------------------------------
bool obstacleDetected() {
  return readDistanceCm() <= OBSTACLE_DISTANCE_CM;
}

void handleObstacle() {
  stopMotors();
  digitalWrite(PIN_ORANGE_LED, HIGH);
  digitalWrite(PIN_GREEN_LED, LOW);

  lcdMessage("OBSTACLE!", "Robot stopped");
  beep(250);

  state = OBSTACLE_STOP;
}

void resumeAfterObstacle() {
  if (!obstacleDetected()) {
    lcdMessage("Path clear", "Resuming...");
    digitalWrite(PIN_GREEN_LED, HIGH);
    delay(500);
    state = DELIVERING;
  }
}

// ------------------------------------------------------------
// Delivery
// ------------------------------------------------------------
void deliverAtTable() {
  stopMotors();
  digitalWrite(PIN_GREEN_LED, LOW);
  digitalWrite(PIN_ORANGE_LED, HIGH);

  lcdMessage(
    "Table " + String(selectedTable),
    "DELIVERED"
  );

  beep(150);
  delay(150);
  beep(150);

  Serial.print("Delivered to table ");
  Serial.println(selectedTable);

  selectedTable = 0;
  state = DELIVERED;
}

void resetToIdle() {
  digitalWrite(PIN_ORANGE_LED, LOW);
  digitalWrite(PIN_GREEN_LED, LOW);
  lcdMessage("Serving Robot", "Press 1-4");
  state = IDLE;
}

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  pinMode(PIN_IR_LEFT, INPUT);
  pinMode(PIN_IR_CENTER, INPUT);
  pinMode(PIN_IR_RIGHT, INPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_ORANGE_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  stopMotors();

  digitalWrite(PIN_RED_LED, HIGH);
  digitalWrite(PIN_GREEN_LED, LOW);
  digitalWrite(PIN_ORANGE_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  Wire.begin();
  lcd.init();
  lcd.backlight();

  SPI.begin();
  rfid.PCD_Init();

  lcdMessage("Serving Robot", "NodeMCU Ready");
  Serial.println("Serving Robot ready.");
  Serial.println("Test: type 1, 2, 3 or 4 in Serial Monitor.");
  delay(1500);

  resetToIdle();
}

// ------------------------------------------------------------
// Main loop
// ------------------------------------------------------------
void loop() {
  switch (state) {

    case IDLE:
      checkForOrder();
      break;

    case DELIVERING:
      if (obstacleDetected()) {
        handleObstacle();
        break;
      }

      followLine();

      if (isSelectedTableTag()) {
        deliverAtTable();
      }

      break;

    case OBSTACLE_STOP:
      resumeAfterObstacle();
      break;

    case DELIVERED:
      delay(1000);
      resetToIdle();
      break;
  }

  delay(20);
}
