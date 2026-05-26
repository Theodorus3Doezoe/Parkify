#pragma once

enum class indicatorState { Free, Reserved, Occupied };

class IStatusIndicator
{
public:
    virtual ~IStatusIndicator() = default;
    virtual void setLightState(indicatorState state) = 0;
};