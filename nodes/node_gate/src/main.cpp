#include <Arduino.h>
#include "../include/CanCommunication.h"
#include "../include/GateController.h"
#include "../include/SimulatedMotor.h"
#include "../include/SimulatedLicencePlateScanner.h"
#include "../include/RFIDScanner.h"
#include "../include/SPI_Setup.h"

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

SPI_Setup SPIsetup;

GateMode mode = ENTRY;
CanCommunication can;
SimulatedMotor motor;
MFRC522 rfid(RFID_SS, RFID_RST);
RFIDScanner scanner(&rfid);

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
    SPIsetup.begin(mode);
    gate.begin();
}

void loop() {
    gate.loop();
}