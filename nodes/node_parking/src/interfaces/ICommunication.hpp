#pragma once
#include <stdint.h>

typedef struct {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[8];
} message_frame;

class ICommunication
{
public:
    virtual ~ICommunication() = default;
    virtual bool tx_message(message_frame msg) = 0;
    virtual message_frame rx_message() = 0;
};