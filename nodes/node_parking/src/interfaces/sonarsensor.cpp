#include "sonarsensor.hpp"

sonarsensor::sonarsensor(uint8_t trig_pin, uint8_t ech_pin, uint16_t trig_distance) : trigger_pin(trig_pin), echo_pin(ech_pin), trigger_distance(trig_distance)
{
    pinMode(trigger_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

bool sonarsensor::sensorTriggered()
{
  digitalWrite(trigger_pin, LOW);
  delay(2);
  
  digitalWrite(trigger_pin, HIGH);
  delay(10);
  digitalWrite(trigger_pin, LOW);
  
  uint16_t duration = pulseIn(echo_pin, HIGH);
  uint16_t distance = (duration * 0.034) / 2;

  if (distance < trigger_distance) {
    return true;
  }

  return false;
}