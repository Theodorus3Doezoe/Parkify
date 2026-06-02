#include <Arduino.h>
#include "../include/CanCommunication.hpp"
#include "../include/GateController.hpp"
#include "../include/SimulatedMotor.hpp"
#include "../include/SimulatedLicencePlateScanner.hpp"

// =====================================================
// KEEP YOUR GLOBALS EXACTLY
// =====================================================

SystemState currentState = INIT;

int validatedParkingID = -1;
String validatedReg = "";

struct can_frame canMsg;

// =====================================================
// SYSTEM OBJECTS
// =====================================================

GateMode mode = ENTRY;
CanCommunication can;
SimulatedMotor motor;
SimulatedLicencePlateScanner scanner;

GateController gate(can, motor, scanner, mode);

// =====================================================

void setup() {

    Serial.begin(115200);
    delay(1000);

    if (mode == ENTRY) 
    {
    Serial.println("STARTING ENTRY GATE");
    } 
    else if (mode == EXIT)
    {
        Serial.println("STARTING EXIT GATE");
    };
    can.begin();
    gate.begin();
}

void loop() {
    gate.loop();
}