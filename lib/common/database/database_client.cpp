#include "database_client.h"
#include "can_comms_protocol.h"
#include "database/database_models.h"
#include <Arduino.h>
#include <algorithm>
#include <cstdint>
#include <cstring>

DatabaseClient::DatabaseClient(MCP2515 &mcp) : _mcp(mcp) {}

bool DatabaseClient::waitForResponse(struct can_frame &frame,
                                     uint8_t expectedCode) {
  unsigned long start = millis();
  while (millis() - start < TIMEOUT_MS) {
    if (_mcp.readMessage(&frame) == MCP2515::ERROR_OK) {
      if (frame.can_id == TX_SESSION_DATA) {
        if (expectedCode == 0 || frame.data[0] == expectedCode ||
            frame.data[0] == 0xFF)
          return true;
      }
    }
    yield();
  }
  return false;
}

std::optional<Data> DatabaseClient::requestSession(uint16_t id) {
  struct can_frame f{};
  f.can_id = REQ_SESSION_DATA;
  f.can_dlc = Data::ID_SIZE;
  memcpy(f.data, &id, Data::ID_SIZE);

  if (_mcp.sendMessage(&f) != MCP2515::ERROR_OK)
    return std::nullopt;

  struct can_frame rx;
  if (waitForResponse(rx) && rx.data[0] != 0xFF) {
    Data d{};
    d.payed = (rx.data[0] == (uint8_t)SessionStatus::PAID);
    memcpy(&d.created_at, &rx.data[1], 4);
    memcpy(d.licenceplate.data(), &rx.data[5], 3);
    d.licenceplate[3] = '\0';
    return d;
  }
  return std::nullopt;
}

bool DatabaseClient::isPaid(uint16_t id) {
  auto session = requestSession(id);
  return session.has_value() && session->payed;
}

bool DatabaseClient::setSessionPaid(uint16_t id) {
  struct can_frame f{};
  f.can_id = TX_VALIDATION;
  f.can_dlc = Data::ID_SIZE;
  memcpy(f.data, &id, Data::ID_SIZE);

  if (_mcp.sendMessage(&f) != MCP2515::ERROR_OK)
    return false;

  struct can_frame rx;
  unsigned long start = millis();
  while (millis() - start < TIMEOUT_MS) {
    if (_mcp.readMessage(&rx) == MCP2515::ERROR_OK) {
      if (rx.can_id == TX_VALIDATED_REG) {
        uint16_t confirmedId = 0;
        memcpy(&confirmedId, rx.data, 2);
        if (confirmedId == id)
          return true;
      }
    }
    yield();
  }
  return false;
}

uint16_t DatabaseClient::requestLowestId() {
  struct can_frame f{};
  f.can_id = REQ_SESSION_DATA;
  f.can_dlc = Data::ID_SIZE;
  uint16_t special = SpecialID::LOWEST_ID;
  memcpy(f.data, &special, Data::ID_SIZE);

  if (_mcp.sendMessage(&f) != MCP2515::ERROR_OK)
    return SpecialID::NOT_FOUND;

  struct can_frame rx;
  if (waitForResponse(rx, 0xFE)) {
    uint16_t id = 0;
    memcpy(&id, &rx.data[1], 2);
    return id;
  }
  return SpecialID::NOT_FOUND;
}

uint16_t DatabaseClient::requestIdByPlate(const char *plate) {
  struct can_frame f{};
  f.can_id = REQ_ID_BY_PLATE;
  f.can_dlc = 3;
  memcpy(f.data, plate, 3);

  if (_mcp.sendMessage(&f) != MCP2515::ERROR_OK)
    return SpecialID::NOT_FOUND;

  struct can_frame rx;
  if (waitForResponse(rx) && rx.data[0] == 0xFE) {
    uint16_t id = 0;
    memcpy(&id, &rx.data[1], 2);
    return id;
  }
  return SpecialID::NOT_FOUND;
}
