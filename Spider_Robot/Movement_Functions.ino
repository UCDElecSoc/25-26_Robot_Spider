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

    // --- Determine Thrust Direction ---
    int legDirection = 1; 
    if (command == 1) legDirection = 1;
    else if (command == -1) legDirection = -1;
    else if (command == 2) legDirection = leg.portStarbound;
    else if (command == -2) legDirection = -leg.portStarbound;

    // --- Pre-calculate positions ---
    int downUpDown = 90 + (45 * leg.portStarbound); 
    int downInOut  = 90 - (30 * leg.portStarbound); 
    int upUpDown   = 90 + (80 * leg.portStarbound); 
    int upInOut    = 90 - (45 * leg.portStarbound);

    int rotCenter   = 90;
    int rotForward  = 90 - (25 * leg.portStarbound * legDirection); 
    int rotBackward = 90 + (25 * leg.portStarbound * legDirection); 

    int currentUpDown, currentInOut, currentRot;

    // --- Apply the Wave Gait ---
    if (st == 0) {
        // IDLE / BOOTUP
        currentUpDown = downUpDown; 
        currentInOut = downInOut; 
        currentRot = rotCenter;
    } else {
        // 1. Assign each leg to one of 4 pairs. 
        // We pair opposite sides/ends so the robot doesn't tip over when lifting!
        int groupOffset = 0;
        if (legIndex == 0 || legIndex == 5) groupOffset = 0;      // Pair 0 lifts on steps 1 & 2
        else if (legIndex == 2 || legIndex == 7) groupOffset = 2; // Pair 1 lifts on steps 3 & 4
        else if (legIndex == 4 || legIndex == 1) groupOffset = 4; // Pair 2 lifts on steps 5 & 6
        else if (legIndex == 6 || legIndex == 3) groupOffset = 6; // Pair 3 lifts on steps 7 & 8

        // 2. Calculate this specific leg's current animation frame (0 to 7)
        int localStep = (st - 1 - groupOffset + 8) % 8;

        // 3. Execute the frame
        switch (localStep) {
            case 0: // LIFT (In the air, pointing back)
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotBackward; break;
            case 1: // SWING (In the air, pointing forward)
                currentUpDown = upUpDown; currentInOut = upInOut; currentRot = rotForward; break;
            case 2: // PLANT (Hit the ground, pointing forward)
                currentUpDown = downUpDown; currentInOut = downInOut; currentRot = rotForward; break;
            
            // Because 6 legs are on the ground at once, we smoothly divide the backward push 
            // across the remaining 5 steps so the robot smoothly glides forward!
            case 3: // PUSH 1
                currentUpDown = downUpDown; currentInOut = downInOut;
                currentRot = rotForward + ((rotBackward - rotForward) * 1 / 5); break;
            case 4: // PUSH 2
                currentUpDown = downUpDown; currentInOut = downInOut;
                currentRot = rotForward + ((rotBackward - rotForward) * 2 / 5); break;
            case 5: // PUSH 3
                currentUpDown = downUpDown; currentInOut = downInOut;
                currentRot = rotForward + ((rotBackward - rotForward) * 3 / 5); break;
            case 6: // PUSH 4
                currentUpDown = downUpDown; currentInOut = downInOut;
                currentRot = rotForward + ((rotBackward - rotForward) * 4 / 5); break;
            case 7: // PUSH 5 (Fully back, ready to lift on next step)
                currentUpDown = downUpDown; currentInOut = downInOut;
                currentRot = rotBackward; break;
        }
    }
// Offset the hind legs (Indices 6 and 7) physically forward by 15 degrees
    if (legIndex == 7 || legIndex == 0) {
        // Since our forward math uses subtraction (90 - 45 * portStarbound),
        // subtracting 15 * portStarbound shifts the entire sweep 15 degrees forward.
        // If they accidentally move backward, just change the "-=" to a "+="!
        currentRot -= (25 * leg.portStarbound); 
    }
    // --- Command the Servos ---
    setServoAngle(leg.rotationPin, currentRot);
    setServoAngle(leg.upDownPin, currentUpDown);
    setServoAngle(leg.inOutPin, currentInOut);
}