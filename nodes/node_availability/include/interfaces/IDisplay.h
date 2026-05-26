#ifndef IDISPLAY_H
#define IDISPLAY_H

#include <stdint.h>

class IDisplay {
public:
    virtual ~IDisplay() = default;
    virtual void init(int sda, int scl) = 0;
    virtual void showAvailability(int available) = 0;
    virtual void clear() = 0;
    virtual void setCursor(uint8_t col, uint8_t page) = 0;
    virtual void printString(const char* str) const = 0;
};

#endif