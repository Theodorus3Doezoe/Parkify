#include "can/CANStateController.h"
#include "can_comms_protocol.h"

CANStateController::CANStateController(MCP2515& mcp) : _mcp(mcp), _currentState(0) {}

void CANStateController::setState(int state) {
    struct can_frame frame;
    frame.can_id = BR_STATE;
    frame.can_dlc = 1;
    frame.data[0] = (uint8_t)state;
    if (_mcp.sendMessage(&frame) == MCP2515::ERROR_OK) {
        _currentState = state;
    }
}

int CANStateController::getState() {
    return _currentState;
}
