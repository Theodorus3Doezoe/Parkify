#include "peripherals/mcp2515_bus.h"
#include "can_config.h"

Mcp2515Bus::Mcp2515Bus(int interruptPin) : mcp2515(CAN_SPI_CS_PIN), intPin(interruptPin) {}

void Mcp2515Bus::init(int miso, int mosi, int sck, int cs) {
    pinMode(intPin, INPUT);
    SPI.begin(sck, miso, mosi, cs);
    mcp2515.reset();
    mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);
    mcp2515.setNormalMode();
}

bool Mcp2515Bus::isMessageAvailable() {
    return digitalRead(intPin) == LOW;
}

void Mcp2515Bus::readMessage(uint32_t &id, uint8_t &data) {
    struct can_frame frame;
    if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
        id = frame.can_id;
        data = frame.data[0];
    }
}
