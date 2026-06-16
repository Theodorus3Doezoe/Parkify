#include "sensors/FireSensor.h"
#include <Arduino.h>

FireSensor::FireSensor(int pin) : _pin(pin) {
    pinMode(_pin, INPUT_PULLUP);
}

bool FireSensor::readSignal() { 
    // Assuming active low button (pressed = LOW)
    return digitalRead(_pin) == LOW; 
}
