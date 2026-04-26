#include "tankDrive.h"

using namespace std;

TankDrive::TankDrive() {
    // Servo attachment happens in begin()
}

void TankDrive::begin() {
    frontLeft.attach(pins[0]);
    frontRight.attach(pins[1]);
    backLeft.attach(pins[2]);
    backRight.attach(pins[3]);
}

// --- Getters ---

bool TankDrive::isPresetActive() const { return presetActive; }
int TankDrive::getCurrentPreset() const { return currentPreset; }
int TankDrive::getCurrentIncrement() const { return currentIncrement; }

// --- Setters ---

void TankDrive::setPresetActive(bool active) { presetActive = active; }
void TankDrive::setCurrentPreset(int preset) { currentPreset = preset; }

// --- Low-level motor control ---

void TankDrive::drive(int lSpeed, int rSpeed) {
    driveAssign(lSpeed, lSpeed, rSpeed, rSpeed);
}

void TankDrive::driveAssign(int speed1, int speed2, int speed3, int speed4) {
    frontLeft.writeMicroseconds(speed1);
    backLeft.writeMicroseconds(speed2);
    frontRight.writeMicroseconds(speed3);
    backRight.writeMicroseconds(speed4);
}

void TankDrive::start() {
    drive(leftSpeed, rightSpeed);
    stopped = false;
}

void TankDrive::stop() {
    drive(STOP_SPEED, STOP_SPEED);
    stopped = true;
}

// --- Speed control ---

void TankDrive::resetSpeed() {
    leftSpeed = STOP_SPEED;
    rightSpeed = STOP_SPEED;
    incrementIndex = 0;
    currentIncrement = INCREMENT_ARRAY[0];
    stopped = true;
    drive(leftSpeed, rightSpeed);
}

void TankDrive::increaseLeftSpeed(int increment) {
    leftSpeed = min(leftSpeed + increment, MAX_SPEED);

    if(!stopped)
    {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::decreaseLeftSpeed(int increment) {
    leftSpeed = max(leftSpeed - increment, MIN_SPEED);
    if(!stopped)
    {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

// NOTE: Right motors are physically inverted, so increasing right speed
//       decreases the microsecond value to produce forward motion.
void TankDrive::increaseRightSpeed(int increment) {
    rightSpeed = max(rightSpeed - increment, MIN_SPEED);
    if(!stopped)
    {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::decreaseRightSpeed(int increment) {
    rightSpeed = min(rightSpeed + increment, MAX_SPEED);

    if(!stopped)
    {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::increaseIncrement() {
    if (incrementIndex < 4) {
        currentIncrement = INCREMENT_ARRAY[++incrementIndex];
    }
    printCurrentSpeeds();
}

void TankDrive::decreaseIncrement() {
    if (incrementIndex > 0) {
        currentIncrement = INCREMENT_ARRAY[--incrementIndex];
    }
    printCurrentSpeeds();
}

void TankDrive::presetForward() {
    leftSpeed = 1560;
    rightSpeed = 1440; 
    drive(leftSpeed, rightSpeed);
    printCurrentSpeeds();
}

void TankDrive::presetBackward() {
    leftSpeed = 1440;
    rightSpeed = 1560; 
    drive(leftSpeed, rightSpeed);
    printCurrentSpeeds();
}

void TankDrive::decreaseLeftSlowly() {
    for (int i = 0; leftSpeed > STOP_SPEED; leftSpeed = max(leftSpeed - currentIncrement, STOP_SPEED)) {
        if (!stopped) {
            drive(leftSpeed, rightSpeed);
        }
        delay(100);
    }
    if (!stopped) {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::decreaseRightSlowly() {
    for (int i = 0; rightSpeed < STOP_SPEED; rightSpeed = min(rightSpeed + currentIncrement, STOP_SPEED)) {
        if (!stopped) {
            drive(leftSpeed, rightSpeed);
        }
        delay(100);
    }
    if (!stopped) {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::accelerationSlowlyRightSide() {
    const int targetRight = 1440;
    stopped = false;
    for (int i = 0; rightSpeed > targetRight; rightSpeed = max(rightSpeed - currentIncrement, targetRight)) {
        if (!stopped) {
            drive(leftSpeed, rightSpeed);
        }
        delay(100);
    }
    if (!stopped) {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

void TankDrive::accelerationSlowlyLeftSide() {
    const int targetLeft = 1560;
    stopped = false;
    for (int i = 0; leftSpeed < targetLeft; leftSpeed = min(leftSpeed + currentIncrement, targetLeft)) {
        if (!stopped) {
            drive(leftSpeed, rightSpeed);
        }
        delay(100);
    }
    if (!stopped) {
        drive(leftSpeed, rightSpeed);
    }
    printCurrentSpeeds();
}

// --- Preset sequences ---

void TankDrive::executeActivePreset() {
    if (!presetActive)
        return;

    // ============================================================================
    // *** PRESET IMPLEMENTATION: Add preset logic here when re-implementing ***
    // *** Populate the switch statement with preset cases (1, 2, 3, etc)      ***
    // ============================================================================

    bool presetComplete = false;

    // switch (currentPreset)
    // {
    //     case 1:
    //         presetComplete = presetForwardWithTurns();
    //         break;
    //     case 2:
    //         presetComplete = presetZigzagPattern();
    //         break;
    //     case 3:
    //         presetComplete = presetFullRotationTest();
    //         break;
    // }

    if (presetComplete) {
        presetActive = false;
        currentPreset = 0;
    }
}

// --- Debug ---

void TankDrive::printCurrentSpeeds() {
    Serial.print("Current Left Speed: ");
    Serial.print(leftSpeed - STOP_SPEED);
    Serial.print(" | Current Right Speed: ");
    Serial.print(STOP_SPEED - rightSpeed);
    Serial.print(" | Current Increment: ");
    Serial.println(currentIncrement);
}
