#pragma once

#include <Arduino.h>
#include <mcp2515.h>

const int CAN_SPI_CS_PIN = 5;

const CAN_SPEED CAN_BITRATE = CAN_500KBPS;
const CAN_CLOCK CAN_CLOCK_SPEED = MCP_8MHZ;

enum SystemState : uint8_t { 
    INIT = 0, 
    CLOSED = 1, 
    OPEN = 2, 
    EMERGENCY = 3 
};
