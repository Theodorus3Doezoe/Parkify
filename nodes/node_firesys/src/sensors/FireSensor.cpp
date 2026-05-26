#include "sensors/FireSensor.h"

FireSensor::FireSensor(int id) : _sensorID(id) {}

bool FireSensor::readSignal() { return false; }
