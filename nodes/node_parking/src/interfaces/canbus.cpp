#include "canbus.hpp"

canbus::canbus(uint8_t cs_pin) : mcp2515(cs_pin)
{
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
}

message_frame canbus::rx_message()
{
    message_frame msg;
    if (mcp2515.readMessage(&Rx) == MCP2515::ERROR_OK) {
        msg.id = Rx.can_id;
        msg.data_length = Rx.can_dlc;
        for (uint8_t i = 0; i < Rx.can_dlc; i++) {
            msg.data[i] = Rx.data[i];
        }
    }
    return msg;
}

bool canbus::tx_message(message_frame msg)
{
    Tx.can_id = msg.id;
    Tx.can_dlc = msg.data_length;
    for (uint8_t i = 0; i < msg.data_length; i++) {
        Tx.data[i] = msg.data[i];
    }
    mcp2515.sendMessage(&Tx);
    return true;
}