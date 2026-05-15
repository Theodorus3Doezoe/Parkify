#pragma once

#include <Arduino.h>

class IInputReader {
public:
    virtual ~IInputReader() {}
    virtual String readInput() = 0;
};
