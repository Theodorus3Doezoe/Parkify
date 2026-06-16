#include "can/CANStateController.h"
#include "can_comms_protocol.h"

CANStateController::CANStateController(MCP2515& mcp, IDisplay& display) 
    : _mcp(mcp), _display(display), _currentState(0) {}

void CANStateController::setState(int state) {
    struct can_frame frame;
    frame.can_id = BR_STATE;
    frame.can_dlc = 1;
    frame.data[0] = (uint8_t)state;
    
    MCP2515::ERROR err = _mcp.sendMessage(&frame);
    if (err == MCP2515::ERROR_OK) {
        _currentState = state;
        _display.logMessage("CAN: State sent " + String(state));
    } else {
        _display.logMessage("CAN: Send Error " + String((int)err));
    }
}

int CANStateController::getState() {
    return _currentState;
}
