#pragma once
#include "Ispot.h"
#include <Arduino.h>
#include "IOccupancySensor.h"
#include "IStatusIndicator.h"

class spot : public Ispot
{
private:
    IOccupancySensor* sensor;
    IStatusIndicator* indicator;

    uint16_t id;
    uint16_t priority;
    uint32_t nextUpdate;
    spotState state = spotState::Free;
    uint8_t reservationLevel;
    uint32_t reservationExpiration;

    void setIndicator(indicatorState state);

public:
    spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator);
    bool update();
    bool getOccupancy();
    uint16_t getId();
    uint16_t getPriority();
    spotState getState();
    void setReservation(bool set, uint8_t level);
    uint8_t getReservationLevel();
};