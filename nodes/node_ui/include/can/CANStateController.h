#pragma once

#include <mcp2515.h>
#include "interfaces/IStateController.h"
#include "interfaces/IDisplay.h"

class CANStateController : public IStateController {
private:
    MCP2515& _mcp;
    IDisplay& _display;
    int _currentState;
public:
    CANStateController(MCP2515& mcp, IDisplay& display);
    void setState(int state) override;
    int getState() override;
};
