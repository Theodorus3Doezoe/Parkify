#ifndef IBUS_H
#define IBUS_H

#include <stdint.h>

class IBus {
public:
    virtual ~IBus() = default;
    virtual void init(int miso, int mosi, int sck, int cs) = 0;
    virtual bool isMessageAvailable() = 0;
    virtual void readMessage(uint32_t &id, uint8_t &data) = 0;
};

#endif