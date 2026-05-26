#include "can/CANGateController.h"
#include "can/CANSpotManager.h"
#include "can/CANStateController.h"
#include "can_config.h"
#include "controllers/UIController.h"
#include "io/ConsoleDisplay.h"
#include "io/ConsoleInputReader.h"
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(CAN_SPI_CS_PIN);

ConsoleInputReader inputReader;
ConsoleDisplay display;
CANStateController stateController(mcp2515);
CANSpotManager spotManager(mcp2515);
CANGateController gateController(mcp2515);

UIController uiController(mcp2515, inputReader, display, stateController,
                          spotManager, gateController);

void setupCanBus() {
  mcp2515.reset();
  if (mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED) == MCP2515::ERROR_OK) {
    if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
      Serial.println("ERR: CAN Mode");
    }
  } else {
    Serial.println("ERR: MCP2515");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("UI Node Ready.");
  setupCanBus();

  Serial.println("\nCommands: [O]pen, [C]lose, [E]mergency, [D]ashboard\n");
}

void loop() { uiController.run(); }
