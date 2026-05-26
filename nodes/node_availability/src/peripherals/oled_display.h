#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "interfaces/IDisplay.h"
#include <Arduino.h>

class OledDisplay : public IDisplay {
private:
    uint8_t i2cAddress;
    void sendCommand(uint8_t command) const;

public:
    OledDisplay(uint8_t address = 0x3D);
    void init(int sda, int scl) override;
    void showAvailability(int available) override;
    void clear() override;
    void setCursor(uint8_t col, uint8_t page) override;
    void printString(const char* str) const override;
};

#endif