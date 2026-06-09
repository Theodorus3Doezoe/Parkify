#include "ICommunication.h"
#include "can_config.h"

class canbus : public ICommunication {
private:
    can_frame Rx;
    can_frame Tx;
    MCP2515 mcp2515;
public:
    canbus(uint8_t cs_pin);
    bool tx_message(message_frame msg) override;
    bool rx_message(message_frame* msg) override;
};