#include "ledIndicator.hpp"

ledIndicator::ledIndicator(uint8_t green_pin, uint8_t blue_pin, uint8_t red_pin) : green_led(green_pin), blue_led(blue_pin), red_led(red_pin)
{
    pinMode(green_led, OUTPUT);
    pinMode(blue_led, OUTPUT);
    pinMode(red_led, OUTPUT);
}

void ledIndicator::setLightState(indicatorState set)
{
    if (state != set) {
        state = set;

        switch (state)
        {
        case indicatorState::Free:
            digitalWrite(green_led, HIGH);
            digitalWrite(blue_led, LOW);
            digitalWrite(green_led, LOW);
            break;

        case indicatorState::Reserved:
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, HIGH);
            digitalWrite(green_led, LOW);
            break;

        case indicatorState::Occupied:
            digitalWrite(green_led, LOW);
            digitalWrite(blue_led, LOW);
            digitalWrite(green_led, HIGH);
            break;
        
        default:
            break;
        }
    }
}