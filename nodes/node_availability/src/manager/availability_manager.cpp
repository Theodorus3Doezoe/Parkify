#include "manager/availability_manager.h"
#include <Arduino.h>
#include "can_comms_protocol.h"

AvailabilityManager::AvailabilityManager(IDisplay& display, IBus& bus, int capacity)
    : oled(display), canBus(bus), totalSpots(capacity), currentOccupied(0) {}

void AvailabilityManager::setup() {
    oled.init(16, 22);
    canBus.init(19, 23, 18, 5);

    oled.clear();
    oled.setCursor(32, 1);
    oled.printString("SYSTEM OK");

    delay(2000);
    oled.showAvailability(totalSpots - currentOccupied);
}

void AvailabilityManager::run() {
    if (canBus.isMessageAvailable()) {
        uint32_t id = 0;
        uint8_t data = 0;
        canBus.readMessage(id, data);

        Serial.print("CAN RX - ID: 0x");
        Serial.print(id, HEX);
        Serial.print(", Data: ");
        Serial.println(data);

        if (id == BR_ENTRY_NOTICE) { // Entry
            if (currentOccupied < totalSpots) {
                currentOccupied++;
                oled.showAvailability(totalSpots - currentOccupied);
            }
        } else if (id == BR_EXIT) { // Exit
            if (currentOccupied > 0) {
                currentOccupied--;
                oled.showAvailability(totalSpots - currentOccupied);
            }
        }
    }
}
