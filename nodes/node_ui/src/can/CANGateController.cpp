#include "can/CANGateController.h"
#include <Arduino.h>

CANGateController::CANGateController(MCP2515& mcp) : _mcp(mcp) {}

void CANGateController::openGate(int gateID) {
}

void CANGateController::closeGate(int gateID) {
}
