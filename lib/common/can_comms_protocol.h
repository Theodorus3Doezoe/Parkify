#pragma once

enum CAN_MessageID {
    REQ_OCCUPANCY = 1,
    TX_OCCUPANCY = 2,
    BR_ENTRY_NOTICE = 3,
    TX_ENTRY_REGISTRATION = 4,
    BR_NODE_PRIORITY = 5,
    TX_SPOT_INFO = 6,
    BR_STATE = 7,
    TX_VALIDATION = 8,
    TX_VALIDATED_REG = 9,
    BR_EXIT = 10
};
