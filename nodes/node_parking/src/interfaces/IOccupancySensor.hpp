#pragma once

class IOccupancySensor
{
public:
    virtual ~IOccupancySensor() = default;
    virtual bool sensorTriggered() = 0;
};