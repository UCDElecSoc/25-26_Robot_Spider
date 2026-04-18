
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
    uint8_t dpad = ctl->dpad();
    int stepDelay = 80; 

    // --- DPAD WALKING CONTROLS ---
    if (dpad == 0x01) {         // D-PAD UP
        walk(1);                
        delay(stepDelay);
    } 
    else if (dpad == 0x02) {    // D-PAD DOWN
        walk(-1);               
        delay(stepDelay);
    } 
    else if (dpad == 0x04) {    // D-PAD RIGHT
        walk(2);                
        delay(stepDelay);
    } 
    else if (dpad == 0x08) {    // D-PAD LEFT
        walk(-2);               
        delay(stepDelay);
    } 
    
    // --- FACE BUTTON WAVE CONTROLS ---
    else if (ctl->a()) {        // 'A' Button Pressed (Cross on PS)
        waveLeg(4);             // Wave Leg 4
        // The wave function has its own delays, so we don't need one here
    }
    else if (ctl->b()) {        // 'B' Button Pressed (Circle on PS)
        waveLeg(3);             // Wave Leg 5
    }
    else if (ctl->x()) {        // 'X' Button Pressed (Square on PS)
        rippleWave();           // Do the stadium wave!
    }

    // --- TRIGGER KICK CONTROLS ---
    else if (ctl->l2()) {  // Left Trigger (L2) pressed past threshold
        kickLeg(3);             // Left Trigger kicks Leg 3
    }
    else if (ctl->r2()) {  // Right Trigger (R2) pressed past threshold
        kickLeg(4);             // Right Trigger kicks Leg 4
    }
    
    // --- IDLE STATE ---
    else {
        Ready();
        delay(stepDelay); 
    }
}
