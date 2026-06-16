#include "io/ConsoleDisplay.h"
#include "can_config.h"

ConsoleDisplay::ConsoleDisplay() {
    for (int i = 0; i < MAX_SPOTS; i++) {
        _spots[i] = {-1, false};
    }
}

void ConsoleDisplay::showState(int state) {
    if (_currentState != state) {
        _currentState = state;
        _isDirty = true;
    }
}

void ConsoleDisplay::showVehicleCount(int count) {
    if (_vehicleCount != count) {
        _vehicleCount = count;
        _isDirty = true;
    }
}

void ConsoleDisplay::showSpotOverview(const SpotStatus* spots, int count) {
    for (int i = 0; i < count; i++) {
        bool found = false;
        for (int j = 0; j < _numSpots; j++) {
            if (_spots[j].id == spots[i].id) {
                if (_spots[j].occupied != spots[i].occupied) {
                    _spots[j].occupied = spots[i].occupied;
                    _isDirty = true;
                }
                found = true;
                break;
            }
        }
        if (!found && _numSpots < MAX_SPOTS) {
            _spots[_numSpots++] = spots[i];
            _isDirty = true;
        }
    }
}

void ConsoleDisplay::showGateStatus(int gateID, bool open) {
    logMessage("Gate " + String(gateID) + ": " + (open ? "OPEN" : "CLOSED"));
}

void ConsoleDisplay::showGateMode(int gateID, int mode) {
    if (gateID == 1) _gate1Mode = mode;
    else if (gateID == 2) _gate2Mode = mode;
    _isDirty = true;
}

void ConsoleDisplay::showError(String message) {
    logMessage("ERR: " + message);
}

void ConsoleDisplay::logMessage(String message) {
    _logs[_logIndex % 5] = message;
    _logIndex++;
    _isDirty = true;
}

void ConsoleDisplay::render() {
    if (!_isDirty) return;

    // \033[?25l = Hide cursor
    // \033[H = Cursor to top-left
    Serial.print("\033[?25l\033[H");

    Serial.println("\033[33m╔════════════════ PARKING GARAGE DASHBOARD ════════════════╗\033[0m");
    
    // Status Row
    Serial.print("  State: ");
    switch (_currentState) {
        case SystemState::OPEN:      Serial.print("\033[32mOPEN     \033[0m"); break;
        case SystemState::CLOSED:    Serial.print("\033[31mCLOSED   \033[0m"); break;
        case SystemState::EMERGENCY: Serial.print("\033[31;1;5mEMERGENCY\033[0m"); break;
        default:                     Serial.print("UNKNOWN  "); break;
    }
    Serial.print(" | Vehicles: ");
    Serial.print(_vehicleCount);
    Serial.println("                     ");

    // Gate Mode Row
    Serial.print("  Gate 1: ");
    Serial.print(_gate1Mode == 0 ? "ENTRY" : "EXIT ");
    Serial.print(" | Gate 2: ");
    Serial.print(_gate2Mode == 0 ? "ENTRY" : "EXIT ");
    Serial.println("                     ");

    Serial.println("╟──────────────────────────────────────────────────────────╢");
    
    // Spots
    Serial.print("  Spots: ");
    for (int i = 0; i < _numSpots; i++) {
        if (_spots[i].occupied) {
            Serial.printf("\033[31m[%d:X] \033[0m", _spots[i].id);
        } else {
            Serial.printf("\033[32m[%d: ] \033[0m", _spots[i].id);
        }
    }
    Serial.println("                                         ");
    
    Serial.println("╟───────────────────────── LOGS ───────────────────────────╢");
    for (int i = 0; i < 5; i++) {
        int idx = (_logIndex > 5) ? (_logIndex - 5 + i) % 5 : i;
        Serial.print("  > ");
        Serial.print(_logs[idx]);
        Serial.println("                                               ");
    }

    Serial.println("\033[33m╚══════════════════════════════════════════════════════════╝\033[0m");
    Serial.print("  [O/C/E] State | [G/H] G1 O/C | [J/K] G2 O/C | [1/2] G1/2 Toggle");
    
    _isDirty = false;
}
