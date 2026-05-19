#pragma once

#include "interfaces/IFireSensor.h"
#include "interfaces/IEmergencyBroadcaster.h"

class FireDetectionSystem {
private:
    bool _fireDetected;
    IFireSensor& _sensor;
    IEmergencyBroadcaster& _broadcaster;

public:
    FireDetectionSystem(IFireSensor& s, IEmergencyBroadcaster& b);
    void checkSensors();
    bool isFireDetected();
};
