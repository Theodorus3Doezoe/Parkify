#include "IStatusIndicator.h"
#include <stdint.h>
#include <Arduino.h>
#include "definitions.h"

class singleLedIndicator : public IStatusIndicator {
private:
    indicatorState state = indicatorState::Free;
    indicatorState prevState = indicatorState::Occupied;
    uint8_t led;
    uint32_t timeToChange;

    void reservationBlink();
public:
    singleLedIndicator(uint8_t led_pin);
    void setLightState(indicatorState set) override;
    void update() override;
};