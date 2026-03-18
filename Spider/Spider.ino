#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --- ESP32-S3 CAMERA BOARD I2C PINS (Front of Board) ---
// Using GPIO 21 (SDA) and GPIO 47 (SCL) as they are easily accessible
// and generally free for use on ESP32-S3 camera boards [citation:2][citation:8].
#define I2C_SDA 21
#define I2C_SCL 47

// Create two PCA9685 objects with different I2C addresses
Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x60);

// Servo configuration
#define SERVOMIN  100  // Minimum pulse length (approximately 0 degrees)
#define SERVOMAX  500  // Maximum pulse length (approximately 180 degrees)
#define SERVOMID  300  // Middle pulse length (approximately 90 degrees)
#define SERVO_FREQ 50  // 50Hz for standard servos
#define SERVOS_PER_BOARD 16

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing PCA9685 boards on ESP32-S3...");

  // Initialize I2C with the front-pin configuration
  Wire.begin(I2C_SDA, I2C_SCL);
  
  Serial.print("Using I2C pins - SDA: GPIO");
  Serial.print(I2C_SDA);
  Serial.print(", SCL: GPIO");
  Serial.println(I2C_SCL);

  // Initialize both PCA9685 boards
  pca1.begin();
  pca1.setPWMFreq(SERVO_FREQ);
  
  pca2.begin();
  pca2.setPWMFreq(SERVO_FREQ);
  
  Serial.println("PCA9685 boards initialized!");

  // Quick I2C scanner to verify connections
  scanI2C();
  
  // Set all servos to 90-degree (middle) positions
  Serial.println("Setting all servos to 90-degree (middle) positions...");
  setAllServosToMiddle();
  Serial.println("All servos set to middle position!");
}

void scanI2C() {
  Serial.println("Scanning I2C devices...");
  byte count = 0;
  
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      if (address == 0x40) Serial.print(" (PCA9685 #1)");
      if (address == 0x60) Serial.print(" (PCA9685 #2)");
      Serial.println();
      count++;
    }
  }
  if (count == 0) {
    Serial.println("No I2C devices found. Check wiring and pull-up resistors!");
  } else {
    Serial.print("Found ");
    Serial.print(count);
    Serial.println(" device(s)");
  }
}

// Function to set a specific servo to a specific angle
void setServoAngle(Adafruit_PWMServoDriver &pca, int servoNum, int angle) {
  // Constrain angle to valid range
  angle = constrain(angle, 0, 180);
  // Map angle to pulse width
  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pca.setPWM(servoNum, 0, pulse);
}

// Function to set all servos on both boards to middle position (90 degrees)
void setAllServosToMiddle() {
  // Calculate middle pulse once
  uint16_t middlePulse = SERVOMID;  // Approximately 300 for 90 degrees
  
  // Set all servos on first board
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca1.setPWM(servoNum, 0, middlePulse);
    delay(10); // Small delay to avoid overwhelming the I2C bus
  }
  
  // Set all servos on second board
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca2.setPWM(servoNum, 0, middlePulse);
    delay(10); // Small delay to avoid overwhelming the I2C bus
  }
}

// Alternative function that uses angle mapping for more precise control
void setAllServosToMiddlePrecise() {
  setAllServosToAngle(90); // 90 degrees is middle position
}

// Function to set all servos to a specific angle
void setAllServosToAngle(int angle) {
  // Constrain angle to valid range
  angle = constrain(angle, 0, 180);
  
  // Map angle to pulse width
  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  
  Serial.print("Setting all servos to ");
  Serial.print(angle);
  Serial.print(" degrees (pulse: ");
  Serial.print(pulse);
  Serial.println(")");
  
  // Set all servos on first board
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca1.setPWM(servoNum, 0, pulse);
    delay(5); // Small delay to avoid overwhelming the I2C bus
  }
  
  // Set all servos on second board
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca2.setPWM(servoNum, 0, pulse);
    delay(5); // Small delay to avoid overwhelming the I2C bus
  }
}

// Function to test individual servo movement
void testIndividualServos() {
  Serial.println("Testing individual servos on both boards...");
  
  // Test first board servos
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    Serial.print("Testing PCA9685 #1, Servo ");
    Serial.println(servoNum);
    
    // Move to 90 degrees
    setServoAngle(pca1, servoNum, 90);
    delay(500);
    
    // Move to 45 degrees
    setServoAngle(pca1, servoNum, 45);
    delay(500);
    
    // Return to 90 degrees
    setServoAngle(pca1, servoNum, 90);
    delay(500);
  }
  
  // Test second board servos
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    Serial.print("Testing PCA9685 #2, Servo ");
    Serial.println(servoNum);
    
    // Move to 90 degrees
    setServoAngle(pca2, servoNum, 90);
    delay(500);
    
    // Move to 135 degrees
    setServoAngle(pca2, servoNum, 135);
    delay(500);
    
    // Return to 90 degrees
    setServoAngle(pca2, servoNum, 90);
    delay(500);
  }
}

void loop() {
  // By default, do nothing - servos stay at 90 degrees
  // Uncomment the sections below for different behaviors
  
  /*
  // Option 1: Continuous slow sweep
  for (int angle = 0; angle <= 180; angle += 5) {
    setAllServosToAngle(angle);
    delay(50);
  }
  for (int angle = 180; angle >= 0; angle -= 5) {
    setAllServosToAngle(angle);
    delay(50);
  }
  */
  
  /*
  // Option 2: Test pattern every 10 seconds
  static unsigned long lastTest = 0;
  if (millis() - lastTest > 10000) { // Every 10 seconds
    lastTest = millis();
    
    // Quick test sequence
    setAllServosToAngle(45);  // Move all to 45 degrees
    delay(1000);
    setAllServosToAngle(135); // Move all to 135 degrees
    delay(1000);
    setAllServosToAngle(90);  // Return to middle
  }
  */
  
  // Default: Do nothing, keep servos at 90 degrees
  delay(1000); // Small delay to prevent watchdog issues
}