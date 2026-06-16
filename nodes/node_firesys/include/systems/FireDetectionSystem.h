#pragma once

#include "interfaces/IFireSensor.h"
#include "interfaces/IEmergencyBroadcaster.h"
#include "can_config.h"

class FireDetectionSystem {
private:
    bool _fireDetected;
    IFireSensor& _sensor;
    IEmergencyBroadcaster& _broadcaster;

public:
    FireDetectionSystem(IFireSensor& s, IEmergencyBroadcaster& b);
    void checkSensors();
    void reset();
    void handleStateChange(SystemState newState);
    bool isFireDetected();
};
