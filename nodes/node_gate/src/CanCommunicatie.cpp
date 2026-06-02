#include "../include/CanCommunication.hpp"

#define CAN_SCK  18
#define CAN_MISO 19
#define CAN_MOSI 23

MCP2515 mcp2515(CAN_SPI_CS_PIN);

void CanCommunication::begin() {
    SPI.begin(CAN_SCK, CAN_MISO, CAN_MOSI, CAN_SPI_CS_PIN);

    mcp2515.reset();
    mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);
    mcp2515.setNormalMode();

    Serial.println("[SYSTEM] MCP2515 Initialized");
}

bool CanCommunication::read(struct can_frame &frame) {
    return mcp2515.readMessage(&frame) == MCP2515::ERROR_OK;
}

void CanCommunication::send(struct can_frame &frame) {
    mcp2515.sendMessage(&frame);
    Serial.println("[CAN] Sent CAN Message");
}