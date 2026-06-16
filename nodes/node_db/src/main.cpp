#include "can_config.h"
#include "can_db_handler.h"
#include "db_memory.h"
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(CAN_SPI_CS_PIN);
CANDatabaseHandler dbHandler(mcp2515, db);

extern void runTestMenu(MCP2515 &mcp);

void setupCanBus() {
  SPI.begin();
  mcp2515.reset();
  if (mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED) == MCP2515::ERROR_OK) {
    if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
      Serial.println("ERR: CAN Loopback Mode");
    } else {
      Serial.println("CAN Loopback Mode Active");
    }
  } else {
    Serial.println("ERR: MCP2515");
  }
}

#ifndef UNIT_TEST
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--- DB Node Ready ---");
  setupCanBus();

  if (!db.begin()) {
    Serial.println("DB Init Failed!");
  }
}

void loop() {
  dbHandler.checkMessages();
  db.autoSave();
  runTestMenu(mcp2515);
}
#endif
