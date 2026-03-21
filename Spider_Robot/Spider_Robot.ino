/**********************************************************************
  Filename    : Video Web Server
  Description : The camera images captured by the ESP32S3 are displayed on the web page.
  Auther      : www.freenove.com
  Modification: 2022/11/01
**********************************************************************/
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
#include "esp_camera.h"
#include <WiFi.h>
#include "sd_read_write.h"
#include "camera_pins.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Select camera model

#define I2C_SDA 21
#define I2C_SCL 47

#define SERVOMIN  100  // Minimum pulse length (approximately 0 degrees)
#define SERVOMAX  500  // Maximum pulse length (approximately 180 degrees)
#define SERVOMID  300  // Middle pulse length (approximately 90 degrees)
#define SERVO_FREQ 50  // 50Hz for standard servos
#define SERVOS_PER_BOARD 16

Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x60);

const char* ssid     = "Kelvin iPhone";   //input your wifi name
const char* password = "1231qweq";   //input your wifi passwords

void cameraInit(void);
void startCameraServer();
void scanI2C(void);
void calibrate(void);
void setServoAngle(Adafruit_PWMServoDriver &pca, int servoNum, int angle);
void setupServos(void);
void setupCamera(void);
void Ready(void);

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  setupServos();
//  setupCamera();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}


