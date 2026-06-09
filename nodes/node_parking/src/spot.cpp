#include "include/spot.h"

spot::spot(uint16_t initId, uint16_t initPrio, IOccupancySensor* initSensor, IStatusIndicator* initIndicator)
    : id(initId), priority(initPrio), sensor(initSensor), indicator(initIndicator) {}

bool spot::update()
{
    bool update = false;
    if (millis() > nextUpdate) {
        nextUpdate = millis() + SENSOR_UPDATE_INTERVAL;
        bool occupancyStart = getOccupancy();
        
        if (sensor->sensorTriggered()) {
            state = spotState::Occupied;
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

    if (state == spotState::Occupied) {
        indicator->setLightState(indicatorState::Occupied);
    } else if (state == spotState::Reserved) {
        indicator->setLightState(indicatorState::Reserved);
    } else {
        indicator->setLightState(indicatorState::Free);
    }

    indicator->update();

    return update;
}

bool spot::getOccupancy() { return (state == spotState::Occupied); }

uint16_t spot::getId() { return id; }

uint16_t spot::getPriority() { return priority; }

void spot::setIndicator(indicatorState state) { indicator->setLightState(state); }

void spot::setReservation(bool set)
{
    if (set) {
        state = spotState::Reserved;
        reservationExpiration = millis() + 10000;
        Serial.printf("SPOT %d: Reserved until %d\n", getId(), reservationExpiration);
    } else {
        state = spotState::Free;
    }
}