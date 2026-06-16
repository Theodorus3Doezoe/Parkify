#pragma once

#include "database/database_models.h"
#include <cstdint>
#include <mcp2515.h>
#include <optional>

class DatabaseClient {
private:
  MCP2515 &_mcp;
  static constexpr uint32_t TIMEOUT_MS = 1000;
  bool waitForResponse(struct can_frame &frame, uint8_t expectedCode = 0);

public:
  explicit DatabaseClient(MCP2515 &mcp);
  std::optional<Data> requestSession(uint16_t id);
  bool isPaid(uint16_t id);
  bool setSessionPaid(uint16_t id);
  uint16_t requestLowestId();
  uint16_t requestIdByPlate(const char *plate);
};
