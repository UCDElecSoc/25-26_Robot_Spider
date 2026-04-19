void setupServos(){
  Serial.println("Initializing PCA9685 boards on ESP32-S3...");
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

  scanI2C();
  Serial.setDebugOutput(true);
//calibrate(); // Calibartaion (Sets all legs at 90 degrees)
  Ready();
}


// Function to set all servos on both boards to middle position (90 degrees)
void calibrate() {
  uint16_t middlePulse = SERVOMID;  // Approximately 300 for 90 degrees
  
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca1.setPWM(servoNum, 0, middlePulse);
    delay(10);
  }
  for (int servoNum = 0; servoNum < SERVOS_PER_BOARD; servoNum++) {
    pca2.setPWM(servoNum, 0, middlePulse);
    delay(10);
  }
}

// Function to set a specific servo to a specific angle
void setServoAngle(int servoNum, int angle) {
  // Constrain servo number to valid range (1-32)
  servoNum = constrain(servoNum, 1, 32);
  
  // Constrain angle to valid range (0-180)
  angle = constrain(angle, 0, 180);
  
  // Map angle to pulse width
  uint16_t pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  
  // Determine which board and which channel
  if (servoNum <= 16) {
    // First board (PCA9685 #1) - servos 1-16
    int channel = servoNum - 1;
    pca1.setPWM(channel, 0, pulse);
    
  } else {
    // Second board (PCA9685 #2) - servos 17-32
    int channel = servoNum - 17;
    pca2.setPWM(channel, 0, pulse);

  }
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