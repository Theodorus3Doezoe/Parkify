#include "include/singleLedIndicator.h"

singleLedIndicator::singleLedIndicator(uint8_t led_pin) : led(led_pin)
{
    pinMode(led, OUTPUT);
}

void singleLedIndicator::setLightState(indicatorState set)
{
    state = set;
    update();
}

void singleLedIndicator::update()
{
    if (state != prevState) {
        prevState = state;

        switch (state)
        {
        case indicatorState::Free:
            digitalWrite(led, HIGH);
            break;

        case indicatorState::Reserved:
            update();
            break;

        case indicatorState::Occupied:
            digitalWrite(led, LOW);
            break;
        
        default:
            break;
        }
    }
    
    if (state == indicatorState::Reserved) {
        if (millis() > timeToChange) {
            digitalWrite(led, (!digitalRead(led)));
            timeToChange = millis() + BLINK_INTERVAL;
        }
    }
}