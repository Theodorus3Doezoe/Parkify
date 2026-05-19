#include "io/ConsoleDisplay.h"

void ConsoleDisplay::showState(int state) {
    Serial.printf("State: %d\n", state);
}

void ConsoleDisplay::showVehicleCount(int count) {
    Serial.printf("Vehicles: %d\n", count);
}

void ConsoleDisplay::showSpotOverview(const SpotStatus* spots, int count) {
    Serial.println("--- Spots ---");
    for (int i = 0; i < count; i++) {
        Serial.printf("%d: %s\n", spots[i].id, spots[i].occupied ? "OCC" : "FREE");
    }
}

void ConsoleDisplay::showGateStatus(int gateID, bool open) {
    Serial.printf("Gate %d: %s\n", gateID, open ? "OPEN" : "CLOSED");
}

void ConsoleDisplay::showError(String message) {
    Serial.printf("ERR: %s\n", message.c_str());
}
