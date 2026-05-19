#include "can/CANEmergencyBroadcaster.h"
#include "can_comms_protocol.h"
#include "can_config.h"

CANEmergencyBroadcaster::CANEmergencyBroadcaster(MCP2515& mcp) : _mcp(mcp) {}

void CANEmergencyBroadcaster::broadcastEmergency() {
    struct can_frame frame;
    frame.can_id = BR_STATE;
    frame.can_dlc = 1;
    frame.data[0] = SystemState::EMERGENCY;
    _mcp.sendMessage(&frame);
}
