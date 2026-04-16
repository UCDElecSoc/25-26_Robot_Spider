/**********************************************************************
  Project Code      : Robot Spider
  Head Programmer   : Kelvin Gao
  Contributers      : Colm McDermott, Mark Tomas, UCD ElecSoc
**********************************************************************/


#include "robot_config.h" 

#include "esp_camera.h"
#include <WiFi.h>
#include "sd_read_write.h"
#include "camera_pins.h"

// --- Variable Definitions ---
// Actually create the objects and variables in memory here

Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x60);

ControllerPtr myControllers[BP32_MAX_GAMEPADS];


const char* ssid     = "Kelvin iPhone";   
const char* password = "1231qweq";   

Angle angle; // Instantiates the Angle struct

int state = 0;

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
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys(); // Optional: clears previously paired devices
  setupServos();

//  setupCamera();
  delay(1000);
}

void loop() {
  // Print the current state to the computer
  BP32.update();

  Serial.print("Executing State: ");
  Serial.println(state + 1); // +1 just to see it as steps 1-8
  
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] && myControllers[i]->isConnected()) {
        processGamepad(myControllers[i]);
    }
  }
  
  // Slowing it down slightly can also help reduce power spikes
//  delay(100); 

}

