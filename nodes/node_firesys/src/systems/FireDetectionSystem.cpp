#include "systems/FireDetectionSystem.h"

FireDetectionSystem::FireDetectionSystem(IFireSensor& s, IEmergencyBroadcaster& b)
    : _fireDetected(false), _sensor(s), _broadcaster(b) {}

void FireDetectionSystem::checkSensors() {
    if (_sensor.readSignal() && !_fireDetected) {
        _fireDetected = true;
        Serial.println("FIRE DETECTED! Sending emergency signal...");
        _broadcaster.broadcastEmergency();
    }
}

void FireDetectionSystem::reset() {
    if (_fireDetected) {
        Serial.println("Resetting Fire System to Normal.");
        _fireDetected = false;
    }
}

void FireDetectionSystem::handleStateChange(SystemState newState) {
    if (newState == SystemState::OPEN || newState == SystemState::CLOSED) {
        reset();
    }
}

bool FireDetectionSystem::isFireDetected() {
    return _fireDetected;
}
