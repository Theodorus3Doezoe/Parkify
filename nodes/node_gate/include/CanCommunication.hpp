#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include "can_comms_protocol.h"
#include "can_config.h"

// KEEP GLOBAL STRUCT NAME
extern struct can_frame canMsg;

class CanCommunication {
public:
    void begin();
    bool read(struct can_frame &frame);
    void send(struct can_frame &frame);

private:
    void setupHardware();
};