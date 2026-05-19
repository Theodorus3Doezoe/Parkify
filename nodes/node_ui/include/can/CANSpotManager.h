#pragma once

#include <mcp2515.h>
#include "interfaces/ISpotManager.h"

class CANSpotManager : public ISpotManager {
private:
    MCP2515& _mcp;
public:
    CANSpotManager(MCP2515& mcp);
    void reserveSpot(int spotID) override;
    void releaseSpot(int spotID) override;
};
