#include "../include/CanCommunication.h"

#define CAN_SCK  18
#define CAN_MISO 19
#define CAN_MOSI 23

MCP2515 mcp2515(CAN_SPI_CS_PIN);

bool CanCommunication::read(struct can_frame &frame) {
    return mcp2515.readMessage(&frame) == MCP2515::ERROR_OK;
}

void CanCommunication::send(struct can_frame &frame) {
    mcp2515.sendMessage(&frame);
    Serial.println("[CAN] Sent CAN Message");
}