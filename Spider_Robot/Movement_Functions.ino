void Ready() {
  state = 0; // Force state to 0
    for (int i = 0; i < 8; i++) {
        setState(i, state);
    }
}

void walk(int direction) {
    state += direction;
    for (int i = 0; i < 8; i++) {
        setState(i, state);
    }
}

void setState(int legIndex, int st) {
    LegConfig leg = legs[legIndex]; 
    int parity = legIndex % 2; // 0 for even indices (Legs 1,3,5,7), 1 for odd indices (Legs 2,4,6,8)

    // Shortcut: If state is 5, 6, 7, or 8, subtract 4 to reuse cases 1, 2, 3, and 4
    int activeState = st;
    if (activeState >= 5 && activeState <= 8) {
        activeState -= 4; 
    }

    // --- Pre-calculate our targeted positions ---
    
    // 1. Idle Positions
    int idleUpDown = 90 + (45 * leg.portStarbound);
    int idleInOut  = 90 - (20 * leg.portStarbound);
    
    // 2. Lift Positions
    int liftUpDown = 90 + (70 * leg.portStarbound); 
    int liftInOut  = 90 - (45 * leg.portStarbound);

    // Rotation stays at 90 for all these steps based on your instructions
    setServoAngle(leg.rotationPin, 90); 

    // --- Apply the states ---
    
    switch (activeState) {
        case 0: // IDLE (Ready)
        case 2: // RESET TO IDLE
        case 4: // RESET TO IDLE
            setServoAngle(leg.upDownPin, idleUpDown);
            setServoAngle(leg.inOutPin, idleInOut);
            break;

        case 1: 
            // Lift legs "2, 4, 6, 8" (Odd indices -> Parity 1)
            if (parity == 1) {
                setServoAngle(leg.upDownPin, liftUpDown);
                setServoAngle(leg.inOutPin, liftInOut);
            } else {
                // Keep the other legs planted in idle
                setServoAngle(leg.upDownPin, idleUpDown);
                setServoAngle(leg.inOutPin, idleInOut);
            }
            break;

        case 3:
            // Lift legs "1, 3, 5, 7" (Even indices -> Parity 0)
            if (parity == 0) {
                setServoAngle(leg.upDownPin, liftUpDown);
                setServoAngle(leg.inOutPin, liftInOut);
            } else {
                // Keep the other legs planted in idle
                setServoAngle(leg.upDownPin, idleUpDown);
                setServoAngle(leg.inOutPin, idleInOut);
            }
            break;
    }
}