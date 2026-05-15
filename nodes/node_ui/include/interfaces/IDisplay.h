#pragma once

#include <Arduino.h>
#include "models/SpotStatus.h"

class IDisplay {
public:
    virtual ~IDisplay() {}
    virtual void showState(int state) = 0;
    virtual void showVehicleCount(int count) = 0;
    virtual void showSpotOverview(const SpotStatus* spots, int count) = 0;
    virtual void showGateStatus(int gateID, bool open) = 0;
    virtual void showError(String message) = 0;
};
