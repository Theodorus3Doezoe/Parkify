#include "can/CANEmergencyBroadcaster.h"
#include "can_comms_protocol.h"
#include "can_config.h"
#include "sensors/FireSensor.h"
#include "systems/FireDetectionSystem.h"
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(CAN_SPI_CS_PIN);

FireSensor sensor(15);
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

void checkCanMessages() {
  struct can_frame frame;
  while (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    if (frame.can_id == BR_STATE && frame.can_dlc >= 1) {
      SystemState newState = static_cast<SystemState>(frame.data[0]);
      fireSystem.handleStateChange(newState);
    }
  }
}

void loop() {
  fireSystem.checkSensors();
  checkCanMessages();
  delay(100);
}
