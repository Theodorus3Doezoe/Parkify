#pragma once
#include <stdint.h>
#include "definitions.h"

enum spotState { Free, Reserved, Occupied, Error };

class Ispot
{
public:
    ~Ispot() = default;
    virtual bool update() = 0;
    virtual bool getOccupancy() = 0;
    virtual uint16_t getId() = 0;
    virtual uint16_t getPriority() = 0;
    virtual spotState getState() = 0;
    virtual void setReservation(bool set, uint8_t level) = 0;
};