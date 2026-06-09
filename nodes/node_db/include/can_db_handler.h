#pragma once

#include "can_comms_protocol.h"
#include "db_memory.h"
#include "database/database_models.h"
#include <mcp2515.h>

class CANDatabaseHandler {
public:
private:
    MCP2515& _mcp;
  Database &_db;

  void handleEntryRegistration(const struct can_frame &frame);
  void handleValidation(const struct can_frame &frame);
  void handleSessionRequest(const struct can_frame &frame);

  void sendSessionData(uint16_t id);

public:
  CANDatabaseHandler(MCP2515 &mcp, Database &db);

  void checkMessages();
};
