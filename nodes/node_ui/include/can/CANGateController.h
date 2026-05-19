#pragma once

#include <mcp2515.h>
#include "interfaces/IGateController.h"

class CANGateController : public IGateController {
private:
    MCP2515& _mcp;
public:
    CANGateController(MCP2515& mcp);
    void openGate(int gateID) override;
    void closeGate(int gateID) override;
};
