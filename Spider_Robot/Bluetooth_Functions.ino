
// --- Your existing robot variables and functions go here ---
// (legs array, Ready(), walk(), setState(), etc.)

// Array to hold connected controllers

// --- Bluepad32 Callback Functions ---
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("Controller connected, index=%d\n", i);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("Controller disconnected, index=%d\n", i);
            myControllers[i] = nullptr;
            break;
        }
    }
}

// --- Map Controller Inputs to Your Robot ---
void processGamepad(ControllerPtr ctl) {
    // Check D-Pad inputs
    uint8_t dpad = ctl->dpad();
    
    // We add a short delay after each step so the servos have time to physically move 
    // to their targets before the next frame of animation is called.
    int stepDelay = 80; // Adjust this! Smaller = faster walking, Larger = slower/smoother

    if (dpad == 0x01) {         // D-PAD UP
        walk(1);                // Walk Forward
        delay(stepDelay);
    } 
    else if (dpad == 0x02) {    // D-PAD DOWN
        walk(-1);               // Walk Backward
        delay(stepDelay);
    } 
    else if (dpad == 0x04) {    // D-PAD RIGHT
        walk(2);                // Turn Right
        delay(stepDelay);
    } 
    else if (dpad == 0x08) {    // D-PAD LEFT
        walk(-2);               // Turn Left
        delay(stepDelay);
    } 
    else {
        // No D-Pad buttons are being pressed. 
        // Reset to the idle stance.
        Ready();
        delay(stepDelay); 
    }
}

