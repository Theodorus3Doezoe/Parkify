#pragma once

#include "interfaces/IFireSensor.h"

class FireSensor : public IFireSensor {
private:
    int _sensorID;
public:
    FireSensor(int id);
    bool readSignal() override;
};
