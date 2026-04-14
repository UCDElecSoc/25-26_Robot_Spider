void Ready() {
  state = 0; // Force state to 0
    for (int i = 0; i < 8; i++) {
        setState(i, state);
    }
}

void walk(int direction) {
    state += direction;
    
    // Wrap around logic for the 8-step cycle
    // If it goes past 8, loop back to 1
    if (state > 8) {state = 1;}
    // If you ever walk backward and go below 1, loop back to 8
    if (state < 1) {state = 8;}
    for (int i = 0; i < 8; i++) {
        setState(i, state);
    }
}

void setState(int legIndex, int st) {
    LegConfig leg = legs[legIndex]; 
    int parity = legIndex % 2; 

    // --- 1. Pre-calculate the targeted 3D positions ---
    
    // Up/Down & In/Out
    int downUpDown = 90 + (45 * leg.portStarbound); // Planted firmly on ground
    int downInOut  = 90 - (20 * leg.portStarbound); 
    int upUpDown   = 90 + (70 * leg.portStarbound); // Lifted high in the air
    int upInOut    = 90 - (45 * leg.portStarbound);

    // Rotation
    int rotCenter   = 90;
    int rotForward  = 90 + (25 * leg.portStarbound); // Swung forward 45 deg
    int rotBackward = 90 - (25 * leg.portStarbound); // Pushed backward 45 deg

    // Variables to hold the final decision for this specific step
    int currentUpDown, currentInOut, currentRot;

    // --- 2. The 8-Step Walking Gait ---

    if (parity == 0) { 
        // ==========================================
        // GROUP A: EVEN LEGS (0, 2, 4, 6)
        // ==========================================
        switch (st) {
            case 0: // IDLE / BOOTUP
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotCenter; break;
            case 1: // LIFT
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotBackward; break;
            case 2: // SWING FORWARD (In the air)
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotForward; break;
            case 3: // PLANT DOWN 
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            case 4: // REST
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            case 5: // SUPPORT BODY (Odds are lifting)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            case 6: // PUSH BACKWARD (Propels robot forward!)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
            case 7: // SUPPORT BODY (Odds are planting)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
            case 8: // REST
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
        }
    } else { 
        // ==========================================
        // GROUP B: ODD LEGS (1, 3, 5, 7)
        // ==========================================
        switch (st) {
            case 0: // IDLE / BOOTUP
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotCenter; break;
            case 1: // SUPPORT BODY (Evens are lifting)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            case 2: // PUSH BACKWARD (Propels robot forward!)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
            case 3: // SUPPORT BODY (Evens are planting)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
            case 4: // REST
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotBackward; break;
            case 5: // LIFT
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotBackward; break;
            case 6: // SWING FORWARD (In the air)
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotForward; break;
            case 7: // PLANT DOWN
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            case 8: // REST
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
        }
    }

    // --- 3. Command the Servos ---
    setServoAngle(leg.rotationPin, currentRot);
    setServoAngle(leg.upDownPin, currentUpDown);
    setServoAngle(leg.inOutPin, currentInOut);
}