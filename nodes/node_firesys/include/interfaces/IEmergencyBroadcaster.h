#pragma once

class IEmergencyBroadcaster {
public:
    virtual ~IEmergencyBroadcaster() {}
    virtual void broadcastEmergency() = 0;
};
