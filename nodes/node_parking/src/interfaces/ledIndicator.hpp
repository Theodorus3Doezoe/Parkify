#include "IStatusIndicator.hpp"
#include <stdint.h>
#include <Arduino.h>

class ledIndicator : public IStatusIndicator {
private:
    indicatorState state;
    uint8_t green_led;
    uint8_t blue_led;
    uint8_t red_led;
public:
    ledIndicator(uint8_t green_pin, uint8_t blue_pin, uint8_t red_pin);
    void setLightState(indicatorState set) override;
};