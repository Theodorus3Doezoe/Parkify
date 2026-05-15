#pragma once

#include <mcp2515.h>
#include "interfaces/IEmergencyBroadcaster.h"

class CANEmergencyBroadcaster : public IEmergencyBroadcaster {
private:
    MCP2515& _mcp;
public:
    CANEmergencyBroadcaster(MCP2515& mcp);
    void broadcastEmergency() override;
};
