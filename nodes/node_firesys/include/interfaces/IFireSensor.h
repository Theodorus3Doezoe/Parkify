#pragma once

class IFireSensor {
public:
    virtual ~IFireSensor() {}
    virtual bool readSignal() = 0;
};
