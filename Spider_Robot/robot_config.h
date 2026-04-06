#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --- Defines ---
#define CAMERA_MODEL_ESP32S3_EYE
#define I2C_SDA 21
#define I2C_SCL 47

#define SERVOMIN  100  
#define SERVOMAX  500  
#define SERVOMID  300  
#define SERVO_FREQ 50  
#define SERVOS_PER_BOARD 16

// --- Structures ---
struct Angle {
    int forward = 30;
    int backward = 30;
    int up = 30;
    int down = 30;
    int in = 20;
    int out = 20;
};

struct LegConfig {
    int rotationPin;
    int upDownPin;
    int inOutPin;
    int portStarbound; 
};

// --- Extern Variables ---
// (This tells the compiler these variables exist in the main file)
extern Angle angle;
extern LegConfig legs[8];
extern int state;
extern Adafruit_PWMServoDriver pca1;
extern Adafruit_PWMServoDriver pca2;
extern const char* ssid;
extern const char* password;

// --- Function Prototypes ---
void cameraInit(void);
void startCameraServer();
void scanI2C(void);
void calibrate(void);
void setServoAngle(Adafruit_PWMServoDriver &pca, int servoNum, int angle);
void setupServos(void);
void setupCamera(void);
void Ready(void);
void setState(int legIndex, int st);
void walk(int direction);

#endif // ROBOT_CONFIG_H