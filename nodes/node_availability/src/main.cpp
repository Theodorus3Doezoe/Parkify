#include <Arduino.h>
#include "manager/availability_manager.h"
#include "peripherals/oled_display.h"
#include "peripherals/mcp2515_bus.h"

OledDisplay display(0x3D);
Mcp2515Bus bus(21);
AvailabilityManager manager(display, bus, 50);

void setup() {
    Serial.begin(115200);
    manager.setup();
}

void loop() {
    manager.run();
    delay(10);
}