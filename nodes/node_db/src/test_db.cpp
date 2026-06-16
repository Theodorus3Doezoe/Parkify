#include <Arduino.h>
#include <mcp2515.h>
#include "can_comms_protocol.h"
#include "database/database_models.h"
#include "db_memory.h"

static uint16_t lastUsedId = 0;

void displayMenu() {
    Serial.println("\n--- [DB NODE TEST MENU] ---");
    Serial.println("1. Simulate Entry (Register Car)");
    Serial.println("2. Simulate Payment (Validate ID)");
    Serial.println("3. Request Session (Get Data)");
    Serial.println("4. Request Lowest ID");
    Serial.println("5. Database Dump");
    Serial.println("6. Force Flash Save");
    Serial.println("7. Clear Database (Wipe All)");
    Serial.println("M. Show Menu");
    Serial.print("Choice: ");
}

void handleIngang(MCP2515 &mcp) {
    lastUsedId = db.getLowestId();
    struct can_frame frame{};
    frame.can_id = TX_ENTRY_REGISTRATION;
    frame.can_dlc = Data::ID_SIZE + 6; // ID (2) + Plate (4)
    
    memcpy(frame.data, &lastUsedId, Data::ID_SIZE);
    memcpy(frame.data + Data::ID_SIZE, "TEST", 4);
    
    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("\n[TEST] Car registered with ID: %u\n", lastUsedId);
    } else {
        Serial.println("\n[TEST] Error sending entry message");
    }
}

void handlePayment(MCP2515 &mcp) {
    Serial.printf("\nEnter ID to pay (empty for %u): ", lastUsedId);
    while(!Serial.available()) { delay(10); }
    
    String input = Serial.readStringUntil('\n');
    uint16_t id = input.length() > 0 ? (uint16_t)input.toInt() : lastUsedId;

    struct can_frame frame{};
    frame.can_id = TX_VALIDATION;
    frame.can_dlc = Data::ID_SIZE;
    memcpy(frame.data, &id, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("[TEST] Payment sent for ID: %u\n", id);
    }
}

void handleRequest(MCP2515 &mcp) {
    Serial.printf("\nEnter ID to request (empty for %u): ", lastUsedId);
    while(!Serial.available()) { delay(10); }
    
    String input = Serial.readStringUntil('\n');
    uint16_t id = input.length() > 0 ? (uint16_t)input.toInt() : lastUsedId;

    struct can_frame frame{};
    frame.can_id = REQ_SESSION_DATA;
    frame.can_dlc = Data::ID_SIZE;
    memcpy(frame.data, &id, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("[TEST] Request sent for ID: %u\n", id);
    }
}

void handleLowestId(MCP2515 &mcp) {
    struct can_frame frame{};
    frame.can_id = REQ_SESSION_DATA;
    frame.can_dlc = Data::ID_SIZE;
    uint16_t special = SpecialID::LOWEST_ID;
    memcpy(frame.data, &special, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.println("\n[TEST] Lowest ID request sent");
    }
}

void runTestMenu(MCP2515 &mcp) {
    static bool firstRun = true;
    if (firstRun) {
        displayMenu();
        firstRun = false;
    }

    if (Serial.available()) {
        char choice = toupper(Serial.read());
        // Flush rest of buffer
        while(Serial.available()) Serial.read();

        switch (choice) {
            case '1': handleIngang(mcp); break;
            case '2': handlePayment(mcp); break;
            case '3': handleRequest(mcp); break;
            case '4': handleLowestId(mcp); break;
            case '5': db.printDatabase(); break;
            case '6': db.saveToFlash(); break;
            case '7': db.clear(); break;
            case 'M': displayMenu(); break;
            default: if (choice != '\n' && choice != '\r') Serial.println("\nInvalid choice"); break;
        }
        
        if (choice != 'M') {
            delay(500); // Wait for CAN processing
            displayMenu();
        }
    }
}
