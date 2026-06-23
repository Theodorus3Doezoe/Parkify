#include "include/spot.h"

spot::spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator)
    : id(initId), priority(initPrio), sensor(initSensor), indicator(initIndicator) {}

bool spot::update()
{
    bool update = false;
    if (millis() > nextUpdate) {
        nextUpdate = millis() + SENSOR_UPDATE_INTERVAL;
        bool occupancyStart = getOccupancy();
        sensorState trigger = sensor->sensorTriggered();
        
        if (trigger == sensorState::Occupied) {
            state = spotState::Occupied;
        } else if (trigger == sensorState::Sensor_Error) {
            state = spotState::Error;
        } else if (state != spotState::Reserved) {
            state = spotState::Free;
        }

        if (occupancyStart != getOccupancy()) {
            update = true;
        }
    }
    
    if ((millis() > reservationExpiration)  & (state == spotState::Reserved)) {
        Serial.printf("SPOT %d: Reservation expired on %d", getId(), millis());
        state = spotState::Free;
    }

    switch (state)
    {
    case spotState::Occupied:
        indicator->setLightState(indicatorState::Occupied);
        break;
        
    case spotState::Reserved:
        indicator->setLightState(indicatorState::Reserved);
        break;
    
    case spotState::Free:
        indicator->setLightState(indicatorState::Free);
        break;
    
    case spotState::Error:
        indicator->setLightState(indicatorState::ErrorIndicator);
        break;
    }

    indicator->update();
    return update;
}

bool spot::getOccupancy()
{
    if (state == spotState::Free) {
        return false;
    }

    return true;
}

uint16_t spot::getId() { return id; }

uint16_t spot::getPriority() { return priority; }

spotState spot::getState() { return state; }

void spot::setIndicator(indicatorState state) { indicator->setLightState(state); }

void spot::setReservation(bool set, uint8_t level)
{
    if (set) {
        state = spotState::Reserved;
        reservationLevel = level;
        reservationExpiration = millis() + 10000;
        Serial.printf("SPOT %d: Reserved until %d\n", getId(), reservationExpiration);
    } else {
        state = spotState::Free;
    }
}

uint8_t spot::getReservationLevel() { return reservationLevel; }