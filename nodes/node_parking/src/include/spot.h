#pragma once
#include <stdint.h>
#include <Arduino.h>
#include "IOccupancySensor.h"
#include "IStatusIndicator.h"
#include "definitions.h"

enum spotState { Free, Reserved, Occupied };

class spot
{
private:
    IOccupancySensor* sensor;
    IStatusIndicator* indicator;

    uint16_t id;
    uint16_t priority;
    uint32_t nextUpdate;
    spotState state = spotState::Free;
    uint32_t reservationExpiration;

public:
    spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator);
    bool update();
    bool getOccupancy();
    uint16_t getId();
    uint16_t getPriority();
    void setIndicator(indicatorState state);
    void setReservation(bool set);
};