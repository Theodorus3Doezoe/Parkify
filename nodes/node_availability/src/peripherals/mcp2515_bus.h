#ifndef MCP2515_BUS_H
#define MCP2515_BUS_H

#include "interfaces/IBus.h"
#include <Arduino.h>
#include <mcp2515.h>

class Mcp2515Bus : public IBus {
private:
    MCP2515 mcp2515;
    int intPin;

public:
    Mcp2515Bus(int interruptPin);
    void init(int miso, int mosi, int sck, int cs) override;
    bool isMessageAvailable() override;
    void readMessage(uint32_t &id, uint8_t &data) override;
};

#endif