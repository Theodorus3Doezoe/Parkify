#include "IOccupancySensor.hpp"
#include <stdint.h>
#include <Arduino.h>

class sonarsensor : public IOccupancySensor {
private:
    uint8_t trigger_pin;
    uint8_t echo_pin;
    uint16_t trigger_distance;
public:
    sonarsensor(uint8_t trig_pin, uint8_t ech_pin, uint16_t trig_distance);
    bool sensorTriggered() override;
};