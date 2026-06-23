#pragma once

enum class sensorState { Free, Occupied, Sensor_Error };

class IOccupancySensor
{
public:
    virtual ~IOccupancySensor() = default;
    virtual sensorState sensorTriggered() = 0;
};