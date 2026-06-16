#pragma once

#include "interfaces/IDisplay.h"

class ConsoleDisplay : public IDisplay {
private:
    int _currentState = 0;
    int _vehicleCount = 0;
    static const int MAX_SPOTS = 12;
    SpotStatus _spots[MAX_SPOTS];
    int _numSpots = 0;
    
    String _logs[5];
    int _logIndex = 0;
    int _gate1Mode = 0; // 0=Entry, 1=Exit
    int _gate2Mode = 1;
    bool _isDirty = true;

public:
    ConsoleDisplay();
    void showState(int state) override;
    void showVehicleCount(int count) override;
    void showSpotOverview(const SpotStatus* spots, int count) override;
    void showGateStatus(int gateID, bool open) override;
    void showGateMode(int gateID, int mode);
    void showError(String message) override;
    void logMessage(String message) override;
    void render() override;
};
