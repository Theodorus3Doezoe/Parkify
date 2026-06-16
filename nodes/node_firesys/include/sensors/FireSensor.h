#pragma once

#include "interfaces/IFireSensor.h"

class FireSensor : public IFireSensor {
private:
    int _pin;
public:
    FireSensor(int pin);
    bool readSignal() override;
};
