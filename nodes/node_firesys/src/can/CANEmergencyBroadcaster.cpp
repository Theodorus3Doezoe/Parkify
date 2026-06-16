#include "can/CANEmergencyBroadcaster.h"
#include "can_comms_protocol.h"
#include "can_config.h"

CANEmergencyBroadcaster::CANEmergencyBroadcaster(MCP2515 &mcp) : _mcp(mcp) {}

void CANEmergencyBroadcaster::broadcastEmergency() {
  struct can_frame frame;
  frame.can_id = BR_STATE;
  frame.can_dlc = 1;
  frame.data[0] = SystemState::EMERGENCY;
  
  MCP2515::ERROR err = _mcp.sendMessage(&frame);
  if (err == MCP2515::ERROR_OK) {
    Serial.println("CAN: Emergency broadcast sent successfully.");
  } else {
    Serial.print("CAN: Error sending emergency broadcast: ");
    Serial.println((int)err);
  }
}
