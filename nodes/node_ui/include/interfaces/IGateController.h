#pragma once

#include "can_comms_protocol.h"

class IGateController {
public:
    virtual ~IGateController() {}
    virtual void openGate(int gateID) = 0;
    virtual void closeGate(int gateID) = 0;
    virtual void setGateMode(int gateID, GateMode mode) = 0;
};
