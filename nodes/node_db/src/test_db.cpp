#include <Arduino.h>
#include <mcp2515.h>
#include "can_comms_protocol.h"
#include "database/database_models.h"
#include "db_memory.h"

static uint16_t lastUsedId = 0;

void displayMenu() {
    Serial.println("\n--- [DB NODE TEST MENU] ---");
    Serial.println("1. Simuleer Ingang (Nieuwe auto registratie)");
    Serial.println("2. Simuleer Betaling (Valideer sessie)");
    Serial.println("3. Opvragen Sessie Data (Specifiek ID)");
    Serial.println("4. Opvragen Laagste Vrije ID");
    Serial.println("5. Database Dump (Overzicht in geheugen)");
    Serial.println("6. Forceer Save naar Flash");
    Serial.println("7. Wis Database (Flash + Geheugen)");
    Serial.println("M. Toon dit menu opnieuw");
    Serial.print("Keuze: ");
}

void handleIngang(MCP2515 &mcp) {
    lastUsedId = db.getLowestId();
    struct can_frame frame{};
    frame.can_id = TX_ENTRY_REGISTRATION;
    frame.can_dlc = Data::ID_SIZE + 6; // ID (2) + Plate (4)
    
    memcpy(frame.data, &lastUsedId, Data::ID_SIZE);
    memcpy(frame.data + Data::ID_SIZE, "TEST", 4);
    
    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("\n[TEST] Auto aangemeld met ID: %u\n", lastUsedId);
    } else {
        Serial.println("\n[TEST] Fout bij versturen ingang bericht");
    }
}

void handlePayment(MCP2515 &mcp) {
    Serial.printf("\nVoer ID in om te betalen (leeg voor %u): ", lastUsedId);
    while(!Serial.available()) { delay(10); }
    
    String input = Serial.readStringUntil('\n');
    uint16_t id = input.length() > 0 ? (uint16_t)input.toInt() : lastUsedId;

    struct can_frame frame{};
    frame.can_id = TX_VALIDATION;
    frame.can_dlc = Data::ID_SIZE;
    memcpy(frame.data, &id, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("[TEST] Betaling verzonden voor ID: %u\n", id);
    }
}

void handleRequest(MCP2515 &mcp) {
    Serial.printf("\nVoer ID in om op te vragen (leeg for %u): ", lastUsedId);
    while(!Serial.available()) { delay(10); }
    
    String input = Serial.readStringUntil('\n');
    uint16_t id = input.length() > 0 ? (uint16_t)input.toInt() : lastUsedId;

    struct can_frame frame{};
    frame.can_id = REQ_SESSION_DATA;
    frame.can_dlc = Data::ID_SIZE;
    memcpy(frame.data, &id, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.printf("[TEST] Sessie opvraag verzonden voor ID: %u\n", id);
    }
}

void handleLowestId(MCP2515 &mcp) {
    struct can_frame frame{};
    frame.can_id = REQ_SESSION_DATA;
    frame.can_dlc = Data::ID_SIZE;
    uint16_t special = SpecialID::LOWEST_ID;
    memcpy(frame.data, &special, Data::ID_SIZE);

    if (mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.println("\n[TEST] Verzoek voor laagste ID verzonden");
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
            default: if (choice != '\n' && choice != '\r') Serial.println("\nOngeldige keuze"); break;
        }
        
        if (choice != 'M') {
            delay(500); // Wacht even op CAN afhandeling
            displayMenu();
        }
    }
}
