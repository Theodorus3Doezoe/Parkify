#include "spot.hpp"

spot::spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator)
    : id(initId), priority(initPrio), sensor(initSensor), indicator(initIndicator) {}

void spot::update()
{
    occupied = sensor->sensorTriggered();
    
    if (reservationExpiration > millis()) {
        setReservation(false);
    }

    if (reserved) {
        indicator->setLightState(indicatorState::Reserved);
    } else if (occupied) {
        indicator->setLightState(indicatorState::Occupied);
    } else {
        indicator->setLightState(indicatorState::Free);
    }
}

bool spot::getOccupancy() { return sensor->sensorTriggered(); }

uint16_t spot::getId() { return id; }

uint16_t spot::getPriority() { return priority; }

void spot::setIndicator(indicatorState state) { indicator->setLightState(state); }

void spot::setReservation(bool set)
{
    reserved = set;
    if (set) {
        reservationExpiration = millis() + 10000; // 10 seconds
    }
}