#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create two PCA9685 objects with different I2C addresses
Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);  // Default address
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x60);  // Alternate address

// Servo configuration
#define SERVOMIN  100  // Minimum pulse length count (out of 4096)
#define SERVOMAX  500  // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

// Number of servos on each board (0-15 for each PCA9685)
#define SERVOS_PER_BOARD 16

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing PCA9685 boards...");

  // Initialize I2C (ESP32 default pins: SDA=21, SCL=22)
  Wire.begin();
  
  // Initialize both PCA9685 boards
  pca1.begin();
  pca1.setPWMFreq(SERVO_FREQ);
  
  pca2.begin();
  pca2.setPWMFreq(SERVO_FREQ);
  
  Serial.println("PCA9685 boards initialized!");
  delay(1000);
}

void moveAllServosToAngle(int angle) {
  // Convert angle (0-180 degrees) to pulse width
  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  
  Serial.print("Moving all servos to angle: ");
  Serial.print(angle);
  Serial.print(" (Pulse: ");
  Serial.print(pulse);
  Serial.println(")");
  
  // Move all servos on PCA9685 #1 (0-15)
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca1.setPWM(servoNum, 0, pulse);
  }
  
  // Move all servos on PCA9685 #2 (0-15)
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca2.setPWM(servoNum, 0, pulse);
  }
}

void sweepAllServos() {
  Serial.println("Sweeping all servos from 0 to 180 degrees...");
  
  for (int angle = 0; angle <= 180; angle += 10) {
    moveAllServosToAngle(angle);
    delay(100);
  }
  
  for (int angle = 180; angle >= 0; angle -= 10) {
    moveAllServosToAngle(angle);
    delay(100);
  }
}

void moveIndividualServo(int board, int servoNum, int angle) {
  if (servoNum < 0 || servoNum >= SERVOS_PER_BOARD) {
    Serial.println("Invalid servo number (0-15)");
    return;
  }
  
  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  
  if (board == 1) {
    pca1.setPWM(servoNum, 0, pulse);
    Serial.print("Board 1, Servo ");
  } else if (board == 2) {
    pca2.setPWM(servoNum, 0, pulse);
    Serial.print("Board 2, Servo ");
  } else {
    Serial.println("Invalid board number (1 or 2)");
    return;
  }
  
  Serial.print(servoNum);
  Serial.print(" to angle: ");
  Serial.println(angle);
}

void loop() {
  // Example 1: Move all servos together
  Serial.println("\n--- Example 1: All servos to 90 degrees ---");
  moveAllServosToAngle(90);
  delay(2000);
  
  // Example 2: Sweep all servos
  Serial.println("\n--- Example 2: Sweeping all servos ---");
  sweepAllServos();
  delay(2000);
  
  // Example 3: Move individual servos
  Serial.println("\n--- Example 3: Moving individual servos ---");
  
  // Move servo 0 on both boards
  moveIndividualServo(1, 0, 0);
  moveIndividualServo(2, 0, 180);
  delay(1000);
  
  moveIndividualServo(1, 0, 180);
  moveIndividualServo(2, 0, 0);
  delay(1000);
  
  // Wave pattern across servos
  Serial.println("\n--- Example 4: Wave pattern ---");
  for (int i = 0; i < SERVOS_PER_BOARD; i++) {
    moveIndividualServo(1, i, 90 + 90 * sin(i * 0.5));
    moveIndividualServo(2, i, 90 + 90 * cos(i * 0.5));
    delay(100);
  }
  
  delay(2000);
}