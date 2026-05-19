#include "systems/FireDetectionSystem.h"

FireDetectionSystem::FireDetectionSystem(IFireSensor& s, IEmergencyBroadcaster& b)
    : _fireDetected(false), _sensor(s), _broadcaster(b) {}

void FireDetectionSystem::checkSensors() {
    if (_sensor.readSignal() && !_fireDetected) {
        _fireDetected = true;
        _broadcaster.broadcastEmergency();
    }
}

bool FireDetectionSystem::isFireDetected() {
    return _fireDetected;
}
