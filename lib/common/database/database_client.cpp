#include "database_client.h"
#include "can_comms_protocol.h"
#include "database/database_models.h"
#include <algorithm>
#include <cstdint>

DatabaseClient::DatabaseClient(MCP2515 &mcp) : _mcp(mcp) {}

void DatabaseClient::requestSession(uint16_t id) {
  struct can_frame frame{};
  frame.can_id = REQ_SESSION_DATA;
  frame.can_dlc = Data::ID_SIZE;

  std::copy(reinterpret_cast<uint8_t *>(&id),
            reinterpret_cast<uint8_t *>(&id) + Data::ID_SIZE, frame.data);

  _mcp.sendMessage(&frame);
}

void DatabaseClient::notifyPayment(uint16_t id) {
  struct can_frame frame{};
  frame.can_id = TX_VALIDATION;
  frame.can_dlc = Data::ID_SIZE;

  std::copy(reinterpret_cast<uint8_t *>(&id),
            reinterpret_cast<uint8_t *>(&id) + Data::ID_SIZE, frame.data);

  _mcp.sendMessage(&frame);
}

void DatabaseClient::requestLowestId() { requestSession(SpecialID::LOWEST_ID); }
