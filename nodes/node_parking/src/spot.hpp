#include <stdint.h>
#include <Arduino.h>
#include "interfaces/IOccupancySensor.hpp"
#include "interfaces/IStatusIndicator.hpp"

class spot
{
private:
    IOccupancySensor* sensor;
    IStatusIndicator* indicator;

    uint16_t id;
    uint16_t priority;
    bool occupied;
    bool reserved;
    uint16_t reservationExpiration;
public:
    spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator);
    void update();
    bool getOccupancy();
    uint16_t getId();
    uint16_t getPriority();
    void setIndicator(indicatorState state);
    void setReservation(bool set);
};