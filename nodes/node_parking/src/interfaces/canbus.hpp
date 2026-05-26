#include "ICommunication.hpp"
#include "../includes/mcp2515.h"

class canbus : public ICommunication {
private:
    can_frame Rx;
    can_frame Tx;
    MCP2515 mcp2515;
public:
    canbus(uint8_t cs_pin);
    bool tx_message(message_frame) override;
    message_frame rx_message() override;
};