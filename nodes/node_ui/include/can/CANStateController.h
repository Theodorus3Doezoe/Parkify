#pragma once

#include <mcp2515.h>
#include "interfaces/IStateController.h"

class CANStateController : public IStateController {
private:
    MCP2515& _mcp;
    int _currentState;
public:
    CANStateController(MCP2515& mcp);
    void setState(int state) override;
    int getState() override;
};
