#pragma once

enum CAN_MessageID {
  // base system 1 - 9
    REQ_OCCUPANCY = 1,
    TX_OCCUPANCY = 2,
    BR_ENTRY_NOTICE = 3,
    TX_ENTRY_REGISTRATION = 4,
    TX_SPOT_INFO = 5,
    BR_STATE = 6,
    TX_VALIDATION = 7,
    TX_VALIDATED_REG = 8,
    BR_EXIT = 9,

    // DB id's 10 - 19
    REQ_SESSION_DATA = 10,
    TX_SESSION_DATA = 11,
    CMD_GATE_CONTROL = 12,
    REQ_ID_BY_PLATE = 13,

    // Node id's 20 - 29
    NODE_BASE_ID = 20,
};
