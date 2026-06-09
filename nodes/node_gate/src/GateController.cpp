#include "../include/GateController.h"

GateController::GateController(CanCommunication &can, IMotor &motor, ILicencePlateScanner &scanner, GateMode mode)
: can(can), motor(motor), scanner(scanner), mode(mode) {}

void GateController::begin() {

    Serial.println("[GATE] Starting Gate");
    if (mode == ENTRY) {
        delay(1000);
        sendOccupancyRequest();
    }
}

// =====================================================
// SEND FUNCTIONS (UNCHANGED LOGIC)
// =====================================================

void GateController::sendOccupancyRequest() {

    canMsg.can_id  = REQ_OCCUPANCY;
    canMsg.can_dlc = 0;

    can.send(canMsg);
    Serial.println("[GATE] Sent Occupancy Request");
}

void GateController::sendEntryNotice(int parkingID) {

    canMsg.can_id  = BR_ENTRY_NOTICE;
    canMsg.can_dlc = 4;

    memcpy(canMsg.data, &parkingID, sizeof(int));

    can.send(canMsg);
}

void GateController::sendRegistration(String reg) {

    canMsg.can_id = TX_ENTRY_REGISTRATION;

    uint8_t len = reg.length();
    if (len > 8) len = 8;

    canMsg.can_dlc = len;
    memcpy(canMsg.data, reg.c_str(), len);

    can.send(canMsg);
}

void GateController::sendExitBroadcast(int parkingID) {

    canMsg.can_id  = BR_EXIT;
    canMsg.can_dlc = 4;

    memcpy(canMsg.data, &parkingID, sizeof(int));

    can.send(canMsg);
}

// =====================================================
// RECEIVE HANDLERS
// =====================================================

void GateController::handleOccupancy(struct can_frame &frame) {
    Serial.print("[ENTRY] Occupied spots received: ");
    Serial.println(frame.data[0]);
}

void GateController::handleState(struct can_frame &frame) {

    currentState = (SystemState)frame.data[0];

    Serial.print("[SYSTEM] State changed to: ");
    Serial.println(frame.data[0]);
}

void GateController::handleValidation(struct can_frame &frame) {

    memcpy(&validatedParkingID, frame.data, sizeof(int));

    Serial.print("[EXIT] Validation received for ParkingID: ");
    Serial.println(validatedParkingID);

    Serial.println("[EXIT] Gate opening...");
    delay(2000);

    sendExitBroadcast(validatedParkingID);
}

void GateController::handleValidatedReg(struct can_frame &frame) {

    char reg[9];
    memcpy(reg, frame.data, frame.can_dlc);
    reg[frame.can_dlc] = '\0';

    validatedReg = String(reg);

    Serial.print("[EXIT] Validated registration received: ");
    Serial.println(validatedReg);
}

// =====================================================
// MESSAGE ROUTER
// =====================================================

void GateController::processCanMessage(struct can_frame &frame) {

    switch(frame.can_id) {

        case TX_OCCUPANCY:
            if (mode == ENTRY) handleOccupancy(frame);
            break;

        case TX_VALIDATION:
            if (mode == EXIT) handleValidation(frame);
            break;

        case TX_VALIDATED_REG:
            if (mode == EXIT) handleValidatedReg(frame);
            break;

        case BR_STATE:
            handleState(frame);
            break;
    }
}

// =====================================================
// LOOP LOGIC SPLIT
// =====================================================

void GateController::loop() {

    if (can.read(canMsg)) {
        processCanMessage(canMsg);
    }

    entrySimulation();
    exitDebug();
}

// =====================================================
// SIMULATION (ENTRY)
// =====================================================

void GateController::entrySimulation() {

    if (mode == EXIT) return;
    
    String registration = scanner.scan();

    if (registration.length() > 0) {

        int parkingID = random(1, 99);

        sendEntryNotice(parkingID);
        delay(100);

        sendRegistration(registration);

        motor.open();
    }
}

// =====================================================
// DEBUG (EXIT)
// =====================================================

void GateController::exitDebug() {

    if (mode == ENTRY) return;

    static unsigned long lastDebug = 0;

    if (millis() - lastDebug > 15000) {
        Serial.println("[EXIT] Waiting for validation...");
        lastDebug = millis();
    }
}