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

void waveLeg(int legTarget) {
    // 1. Stand in the idle position first so the robot is balanced
    Ready();
    delay(200); 

    // 2. Grab the specific leg's hardware config so we know its pins
    LegConfig leg = legs[legTarget];

    // 3. Calculate the lifted and sweep positions using your portStarbound math
    int liftUpDown = 90 + (80 * leg.portStarbound);  // Matches your 'upUpDown' variable
    int liftInOut  = 90 + (45 * leg.portStarbound);  // Matches your 'upInOut' variable
    int waveForward = 90 - (40 * leg.portStarbound); // Sweep forward limit
    int waveBack    = 90 + (40 * leg.portStarbound); // Sweep backward limit

    // 4. Lift the leg into the air directly using setServoAngle
    setServoAngle(leg.upDownPin, liftUpDown);
    setServoAngle(leg.inOutPin, liftInOut);
    delay(200); // Give the servos time to physically lift

    // 5. Wave it back and forth 3 times
    for (int i = 0; i < 3; i++) {
        setServoAngle(leg.rotationPin, waveForward);
        delay(150);
        
        setServoAngle(leg.rotationPin, waveBack);
        delay(150);
    }

    // 6. Put the leg back down and return to the normal stance
    Ready();
}

void rippleWave() {
    // 1. Plant all feet firmly on the ground first
    Ready();
    delay(200); 

    // 2. Loop through all 8 legs (Indices 0 through 7)
    for (int i = 0; i < 8; i++) {
        LegConfig leg = legs[i];

        // --- Calculate Up and Down limits using your math ---
        int liftUpDown = 90 + (80 * leg.portStarbound); 
        int liftInOut  = 90 - (45 * leg.portStarbound); 
        
        int downUpDown = 90 + (45 * leg.portStarbound);
        int downInOut  = 90 - (30 * leg.portStarbound);

        // --- Lift the Leg ---
        setServoAngle(leg.upDownPin, liftUpDown);
        setServoAngle(leg.inOutPin, liftInOut);
        
        delay(200); // 200ms delay while it is in the air

        // --- Put the Leg Down ---
        setServoAngle(leg.upDownPin, downUpDown);
        setServoAngle(leg.inOutPin, downInOut);
        
        // (Optional) Add a tiny delay here like delay(50); if you want 
        // a slight pause before the next leg instantly shoots up!
    }

    // 3. Ensure everything is perfectly aligned at the end
    Ready();
}

void kickLeg(int legTarget) {
    // 1. Brace for the kick
    Ready();
    delay(100); 

    LegConfig leg = legs[legTarget];

    // 2. Calculate the forward rotation
    int kickRot = 90 - (45 * leg.portStarbound); 

    // 3. Determine the Outward Kick Angle based on which leg is kicking
    int kickInOut;
    if (legTarget == 3) {
        kickInOut = 135; // Left side needs 135 to kick outward
    } else {
        kickInOut = 45;  // Right side is mirrored, so it needs 45 to kick outward
    }

    // 4. STRIKE!
    setServoAngle(leg.rotationPin, kickRot);
    setServoAngle(leg.inOutPin, kickInOut); 
    
    // Hold the kick out for 300 milliseconds
    delay(300); 

    // 5. Snap back to the ready stance
    Ready();
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
        currentRot += (25 * leg.portStarbound); 
    }    
    if (legIndex == 6 || legIndex == 1) {
        // Since our forward math uses subtraction (90 - 45 * portStarbound),
        // subtracting 15 * portStarbound shifts the entire sweep 15 degrees forward.
        // If they accidentally move backward, just change the "-=" to a "+="!
        currentRot += (15 * leg.portStarbound); 
    }
    // --- Command the Servos ---
    setServoAngle(leg.rotationPin, currentRot);
    setServoAngle(leg.upDownPin, currentUpDown);
    setServoAngle(leg.inOutPin, currentInOut);
}