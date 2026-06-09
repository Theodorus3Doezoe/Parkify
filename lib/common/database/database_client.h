#pragma once

#include <cstdint>
#include <mcp2515.h>

class DatabaseClient {
private:
  MCP2515 &_mcp;

public:
  explicit DatabaseClient(MCP2515 &mcp);

  void requestSession(uint16_t id);

  void notifyPayment(uint16_t id);

  void requestLowestId();
};
