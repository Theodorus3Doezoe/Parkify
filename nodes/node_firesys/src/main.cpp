#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include "can_config.h"
#include "sensors/FireSensor.h"
#include "can/CANEmergencyBroadcaster.h"
#include "systems/FireDetectionSystem.h"

MCP2515 mcp2515(CAN_SPI_CS_PIN);

FireSensor sensor(1);
CANEmergencyBroadcaster broadcaster(mcp2515);
FireDetectionSystem fireSystem(sensor, broadcaster);

void setupCanBus() {
    mcp2515.reset();
    if (mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED) == MCP2515::ERROR_OK) {
        mcp2515.setNormalMode();
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Fire System Ready.");
    setupCanBus();
}

void loop() {
    fireSystem.checkSensors();
    delay(100);
}
