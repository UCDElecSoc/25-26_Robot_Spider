void Ready() {
  state = 0; // Force state to 0
    for (int i = 0; i < 8; i++) {
        setState(i, state, 1);
    }
}

void walk(int command) {
    // The animation timeline ALWAYS moves forward to keep the legs in sync
    state += 1; 
    
    // Wrap around for the 8-step cycle
    if (state > 8) {
        state = 1; 
    }

    // Pass both the state AND the command down to the legs
    for (int i = 0; i < 8; i++) {
        setState(i, state, command); 
    }
}

void setState(int legIndex, int st, int command) {
    LegConfig leg = legs[legIndex]; 
    int parity = legIndex % 2; 

    // --- NEW: Determine the leg's thrust direction based on your command ---
    int legDirection = 1; // 1 = Push Forward, -1 = Push Backward

    if (command == 1) {
        legDirection = 1;  // All legs walk forward
    } 
    else if (command == -1) {
        legDirection = -1; // All legs walk backward
    } 
    else if (command == 2) {
        // TURN RIGHT: Port side walks forward (1), Starboard side walks backward (-1)
        legDirection = leg.portStarbound; 
    } 
    else if (command == -2) {
        // TURN LEFT: Port side walks backward (-1), Starboard side walks forward (1)
        legDirection = -leg.portStarbound; 
    }

    // --- Pre-calculate the targeted 3D positions ---
    
    int downUpDown = 90 + (45 * leg.portStarbound); 
    int downInOut  = 90 - (20 * leg.portStarbound); 
    int upUpDown   = 90 + (70 * leg.portStarbound); 
    int upInOut    = 90 - (45 * leg.portStarbound);

    // --- NEW: Multiply the rotation by legDirection so it flips when steering! ---
    int rotCenter   = 90;
    int rotForward  = 90 - (25 * leg.portStarbound * legDirection); 
    int rotBackward = 90 + (25 * leg.portStarbound * legDirection); 

    int currentUpDown, currentInOut, currentRot;

    // ... (Keep your Group A and Group B switch statements exactly the same as before!) ...

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

// --- 3. Apply Custom Hardware Offsets ---
    
    // Offset Leg 4 (Index 4) down by 2 degrees
    if (legIndex == 4) {
        // Because Leg 4 is on the Starboard side, its "down" position is 45 
        // and its "up" position is 20. To push it further down toward the floor, 
        // we add to the angle. (If it moves the wrong way, just change this to -2).
        currentUpDown -= 2; 
    }

    // --- 4. Command the Servos ---
    setServoAngle(leg.rotationPin, currentRot);
    setServoAngle(leg.upDownPin, currentUpDown);
    setServoAngle(leg.inOutPin, currentInOut);
}