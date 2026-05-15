#pragma once

#include "interfaces/IDisplay.h"

class ConsoleDisplay : public IDisplay {
public:
    void showState(int state) override;
    void showVehicleCount(int count) override;
    void showSpotOverview(const SpotStatus* spots, int count) override;
    void showGateStatus(int gateID, bool open) override;
    void showError(String message) override;
};
