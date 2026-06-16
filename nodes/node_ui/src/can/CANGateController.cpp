#include "can/CANGateController.h"
#include "can_comms_protocol.h"
#include <Arduino.h>

CANGateController::CANGateController(MCP2515 &mcp) : _mcp(mcp) {}

void CANGateController::openGate(int gateID) {
  struct can_frame frame;
  frame.can_id = BR_GATE_CONTROL;
  frame.can_dlc = 2;
  frame.data[0] = (uint8_t)gateID;
  frame.data[1] = 1; // 1 = Open
  _mcp.sendMessage(&frame);
}

void CANGateController::closeGate(int gateID) {
  struct can_frame frame;
  frame.can_id = BR_GATE_CONTROL;
  frame.can_dlc = 2;
  frame.data[0] = (uint8_t)gateID;
  frame.data[1] = 0; // 0 = Close
  _mcp.sendMessage(&frame);
}

void CANGateController::setGateMode(int gateID, GateMode mode) {
  struct can_frame frame;
  frame.can_id = BR_GATE_MODE;
  frame.can_dlc = 2;
  frame.data[0] = (uint8_t)gateID;
  frame.data[1] = (uint8_t)mode;
  _mcp.sendMessage(&frame);
}
