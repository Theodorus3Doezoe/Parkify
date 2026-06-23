#pragma once

enum class indicatorState { Free, Reserved, Occupied, ErrorIndicator };

class IStatusIndicator
{
public:
    virtual ~IStatusIndicator() = default;
    virtual void setLightState(indicatorState state) = 0;
    virtual void update() = 0;
};