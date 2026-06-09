#include "IStatusIndicator.h"
#include <stdint.h>
#include <Arduino.h>

class multiLedIndicator : public IStatusIndicator {
private:
    indicatorState state;
    indicatorState prevState;
    uint8_t green_led;
    uint8_t blue_led;
    uint8_t red_led;
public:
    multiLedIndicator(uint8_t green_pin, uint8_t blue_pin, uint8_t red_pin);
    void setLightState(indicatorState set) override;
    void update() override;
};