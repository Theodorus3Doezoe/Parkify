// =====================================================
// This is the code for both the entry and exit gates of the parkify system, at the moment it is all in one big file which should be
// split up into multiple classes (shown in the class diagram in the document).
// As well as having a few key points in the code work differently from how they do right now.
// =====================================================

#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

// =====================================================
// MCP2515 PINS (ESP32)
// =====================================================

#define CAN_CS_PIN 5

// SPI Pins for ESP32
#define CAN_SCK  18
#define CAN_MISO 19
#define CAN_MOSI 23

MCP2515 mcp2515(CAN_CS_PIN);

struct can_frame canMsg;

// =====================================================
// DEVICE MODE
// Change this to true for Entry Gate
// Change this to false for Exit Gate
// =====================================================

bool IS_ENTRY_GATE = true;

// =====================================================
// CAN IDs
// =====================================================

enum CAN_IDS {
    REQ_OCCUPANCY       = 1,
    TX_OCCUPANCY        = 2,
    BR_ENTRY_NOTICE     = 3,
    TX_ENTRY_REG        = 4,
    BR_NODE_PRIORITY    = 5,
    TX_SPOT_INFO        = 6,
    BR_STATE            = 7,
    TX_VALIDATION       = 8,
    TX_VALIDATED_REG    = 9,
    BR_EXIT             = 10
};

// =====================================================
// GARAGE STATES
// =====================================================

enum GarageState {
    INIT = 0,
    CLOSED = 1,
    OPEN = 2,
    EMERGENCY = 3
};

GarageState currentState = INIT;

// =====================================================
// EXIT GATE VARIABLES
// =====================================================

int validatedParkingID = -1;
String validatedReg = "";

// =====================================================
// DEBUG HELPERS
// =====================================================

void printSeparator() {
    Serial.println("------------------------------------------------");
}

void printCanFrame(struct can_frame &frame) {

    Serial.print("[CAN RX] ID: ");
    Serial.print(frame.can_id);

    Serial.print(" | DLC: ");
    Serial.print(frame.can_dlc);

    Serial.print(" | DATA: ");

    for (int i = 0; i < frame.can_dlc; i++) {
        Serial.print(frame.data[i]);
        Serial.print(" ");
    }

    Serial.println();
}

// =====================================================
// SEND FUNCTIONS
// =====================================================

// ID 1
void sendOccupancyRequest() {

    canMsg.can_id  = REQ_OCCUPANCY;
    canMsg.can_dlc = 0;

    MCP2515::ERROR result = mcp2515.sendMessage(&canMsg);

    if (result == MCP2515::ERROR_OK) {
        Serial.println("[ENTRY] Occupancy request sent");
    } else {
        Serial.println("[ENTRY] ERROR sending occupancy request");
    }
}

// ID 3
void sendEntryNotice(int parkingID) {

    canMsg.can_id  = BR_ENTRY_NOTICE;
    canMsg.can_dlc = 4;

    memcpy(canMsg.data, &parkingID, sizeof(int));

    MCP2515::ERROR result = mcp2515.sendMessage(&canMsg);

    if (result == MCP2515::ERROR_OK) {

        Serial.print("[ENTRY] Entry notice sent. ParkingID: ");
        Serial.println(parkingID);

    } else {

        Serial.println("[ENTRY] ERROR sending entry notice");
    }
}

// ID 4
void sendRegistration(String reg) {

    canMsg.can_id = TX_ENTRY_REG;

    uint8_t len = reg.length();

    if (len > 8) {
        len = 8;
    }

    canMsg.can_dlc = len;

    memcpy(canMsg.data, reg.c_str(), len);

    MCP2515::ERROR result = mcp2515.sendMessage(&canMsg);

    if (result == MCP2515::ERROR_OK) {

        Serial.print("[ENTRY] Registration sent: ");
        Serial.println(reg);

    } else {

        Serial.println("[ENTRY] ERROR sending registration");
    }
}

// ID 10
void sendExitBroadcast(int parkingID) {

    canMsg.can_id  = BR_EXIT;
    canMsg.can_dlc = 4;

    memcpy(canMsg.data, &parkingID, sizeof(int));

    MCP2515::ERROR result = mcp2515.sendMessage(&canMsg);

    if (result == MCP2515::ERROR_OK) {

        Serial.print("[EXIT] Exit broadcast sent. ParkingID: ");
        Serial.println(parkingID);

    } else {

        Serial.println("[EXIT] ERROR sending exit broadcast");
    }
}

// =====================================================
// RECEIVE HANDLERS
// =====================================================

// ID 2
void handleOccupancy(struct can_frame &frame) {

    int occupied = frame.data[0];

    Serial.print("[ENTRY] Occupied spots received: ");
    Serial.println(occupied);
}

// ID 7
void handleState(struct can_frame &frame) {

    int state = frame.data[0];

    currentState = (GarageState)state;

    Serial.print("[SYSTEM] State changed to: ");

    switch(currentState) {

        case INIT:
            Serial.println("INIT");
            break;

        case CLOSED:
            Serial.println("CLOSED");
            break;

        case OPEN:
            Serial.println("OPEN");
            break;

        case EMERGENCY:
            Serial.println("EMERGENCY");
            break;

        default:
            Serial.println("UNKNOWN");
            break;
    }
}

// ID 8
void handleValidation(struct can_frame &frame) {

    memcpy(&validatedParkingID, frame.data, sizeof(int));

    Serial.print("[EXIT] Validation received for ParkingID: ");
    Serial.println(validatedParkingID);
}

// ID 9
void handleValidatedReg(struct can_frame &frame) {

    char reg[9];

    memcpy(reg, frame.data, frame.can_dlc);

    reg[frame.can_dlc] = '\0';

    validatedReg = String(reg);

    Serial.print("[EXIT] Validated registration received: ");
    Serial.println(validatedReg);
}

// =====================================================
// PROCESS INCOMING CAN MESSAGES
// =====================================================

void processCanMessage(struct can_frame &frame) {

    printSeparator();

    printCanFrame(frame);

    switch(frame.can_id) {

        // =============================================
        // ENTRY GATE RECEIVES
        // =============================================

        case TX_OCCUPANCY:

            if (IS_ENTRY_GATE) {
                handleOccupancy(frame);
            }

            break;

        // =============================================
        // EXIT GATE RECEIVES
        // =============================================

        case TX_VALIDATION:

            if (!IS_ENTRY_GATE) {

                handleValidation(frame);

                Serial.println("[EXIT] Gate opening...");

                delay(2000);

                sendExitBroadcast(validatedParkingID);
            }

            break;

        case TX_VALIDATED_REG:

            if (!IS_ENTRY_GATE) {
                handleValidatedReg(frame);
            }

            break;

        // =============================================
        // BOTH RECEIVE
        // =============================================

        case BR_STATE:

            handleState(frame);

            break;

        default:

            Serial.println("[SYSTEM] Unknown CAN message");

            break;
    }

    printSeparator();
}

// =====================================================
// SETUP
// =====================================================

void setup() {

    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println();
    Serial.println("======================================");

    if (IS_ENTRY_GATE) {
        Serial.println("STARTING ENTRY GATE");
    } else {
        Serial.println("STARTING EXIT GATE");
    }

    Serial.println("======================================");

    // SPI Setup
    SPI.begin(CAN_SCK, CAN_MISO, CAN_MOSI, CAN_CS_PIN);

    // MCP2515 Setup
    mcp2515.reset();

    mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);

    mcp2515.setNormalMode();

    Serial.println("[SYSTEM] MCP2515 Initialized");
    Serial.println("[SYSTEM] CAN Bus Running");

    // Entry gate startup action
    if (IS_ENTRY_GATE) {

        delay(1000);

        sendOccupancyRequest();
    }
}

// =====================================================
// LOOP
// =====================================================

void loop() {

    // =================================================
    // RECEIVE CAN
    // =================================================

    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

        processCanMessage(canMsg);
    }

    // =================================================
    // ENTRY GATE TEST SIMULATION
    // =================================================

    // The entry gate doesn't generate any realistic mock data at the moment since it can get multiple of the same parking IDs
    // and only contains one registration, which in the end should be randomly generated.

    if (IS_ENTRY_GATE) {

        static unsigned long lastEntry = 0;

        if (millis() - lastEntry > 2500) {

            Serial.println();
            Serial.println("[ENTRY] Simulating vehicle entry");

            // I will change the way the system gives a parking ID to each car individually, by running a counter and then every time a car
            // leaves the garage it will save that car its ID to an array and use those first.
            int parkingID = random(1, 99);

            sendEntryNotice(parkingID);

            delay(100);

            // Registration will be at first randomly generated by a mock system when I update this code, and if that works, I will
            // also make it so it uses an RFID scanner to simulate the licence plate scanner.
            sendRegistration("RNDF95");

            lastEntry = millis();
        }
    }

    // =================================================
    // EXIT GATE TEST INFO
    // =================================================


    // The exit gate doesn't do anything yet at the moment, except pretending as if it opens whenever it receives a validation.
    // In the end it should not open after the validation is received but after it sees the car standing in front of the gate itself.

    if (!IS_ENTRY_GATE) {

        static unsigned long lastDebug = 0;

        if (millis() - lastDebug > 15000) {

            Serial.println();
            Serial.println("[EXIT] Waiting for validation...");

            lastDebug = millis();
        }
    }
}