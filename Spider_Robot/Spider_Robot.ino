/**********************************************************************
  Project Code      : Robot Spider
  Head Programmers  : Kelvin Gao, Colm McDermott
  Contributers      : Mark Tomas, Nick Fu, UCD ElecSoc Members

  Head of Mech      : Liam Scott
  Contributers      : Li You, Zaid Niaz, UCD ElecSoc Members
***********************************************************************
  Live Video Link   : http://172.20.10.11
  Enter pairing mode on a valid BLE controller to connect

  Please note, only either video or bluetooth works, NOT both
***********************************************************************
  Controller Commands:
  D-Pad             : Movement + Rotation
  A                 : Wave Right
  B                 : Wave Left
  X                 : Readjust Legs
  R-Trigger         : Right Kick
  L-Trigger         : Left Kick
**********************************************************************/

#include "robot_config.h" 

#include "esp_camera.h"
#include <WiFi.h>
#include "sd_read_write.h"
#include "camera_pins.h"

// Connect a Hotspot to use the wifi camera
const char* ssid     = "[insert ur ssid]";   
const char* password = "[insert ur hotspot password]";   

// --- Variable Definitions ---
// Actually create the objects and variables in memory here

Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x60);

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
Angle angle; 

int state = 0;
// Legs from port anitclockwise: 0,1,2,3,4,5,6,7
// Diagram:
//   3  2  1  0
//          --> face
//   4  5  6  7  

LegConfig legs[8] = {  // Leg Pins Config (1 = Port, -1 = Starboard)
    {1,  2,  3,   1},  // Leg 0 [Rot Pin, Up/Down Pin, In/Out Pin, Port(1)]
    {5,  6,  7,   1},  // Leg 1
    {9, 10, 11,  1},  // Leg 2
    {13, 14, 15,  1},  // Leg 3
    {20, 19, 18, -1},  // Leg 4 [Rot Pin, Up/Down Pin, In/Out Pin, Starboard(-1)]
    {23, 22, 21, -1},  // Leg 5
    {28, 27, 26, -1},  // Leg 6
    {32, 31, 30, -1}   // Leg 7
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Bluetooth Stuff (COMMENT TO USE CAMERA)
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys(); 

//setupCamera();  //UNCOMMENT FOR CAMERA

  setupServos();
  delay(1000);
}

void loop() {
  // Print the current state to the computer
  Serial.print("Executing State: ");
  Serial.println(state + 1); 
  
  //Bluetooth Gamepad check (COMMENT FOR CAMERA)
  BP32.update();
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] && myControllers[i]->isConnected()) {
        processGamepad(myControllers[i]);
    }
  }
}

