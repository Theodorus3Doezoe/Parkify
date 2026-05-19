#pragma once

class IGateController {
public:
    virtual ~IGateController() {}
    virtual void openGate(int gateID) = 0;
    virtual void closeGate(int gateID) = 0;
};
