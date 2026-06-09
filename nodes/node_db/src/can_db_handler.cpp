#include "can_db_handler.h"
#include <Arduino.h>

CANDatabaseHandler::CANDatabaseHandler(MCP2515 &mcp, Database &db)
    : _mcp(mcp), _db(db) {}

void CANDatabaseHandler::checkMessages() {
  struct can_frame frame;
  while (_mcp.readMessage(&frame) == MCP2515::ERROR_OK) {
    switch (frame.can_id) {
    case TX_ENTRY_REGISTRATION:
      handleEntryRegistration(frame);
      break;
    case TX_VALIDATION:
      handleValidation(frame);
      break;
    case REQ_SESSION_DATA:
      handleSessionRequest(frame);
      break;
    case TX_SESSION_DATA:
      // For loopback testing/diagnostics
      {
        uint8_t index = frame.data[0];
        if (index == 0xFF) {
          Serial.println("[CAN DB] LOOPBACK: Session NOT FOUND");
        } else if (index == 0xFE) {
          uint16_t assignedId = 0;
          memcpy(&assignedId, &frame.data[1], 2);
          Serial.printf("[CAN DB] LOOPBACK: Assigned ID received: %u\n",
                        assignedId);
        } else if (index == 0) {
          uint8_t status = frame.data[1];
          uint32_t timestamp = 0;
          memcpy(&timestamp, &frame.data[2], 4);
          Serial.printf(
              "[CAN DB] LOOPBACK: Frame 0 - Status: %u, Created: %u\n", status,
              timestamp);
        } else {
          Serial.printf("[CAN DB] LOOPBACK: Frame %u received\n", index);
        }
      }
      break;
    default:
      break;
    }
  }
}

void CANDatabaseHandler::handleEntryRegistration(
    const struct can_frame &frame) {
  if (frame.can_dlc < Data::ID_SIZE)
    return;

  uint16_t id = 0;
  std::copy(frame.data, frame.data + Data::ID_SIZE,
            reinterpret_cast<uint8_t *>(&id));

  Data newData{};
  newData.created_at = 1704067200; // Dummy Unix: 2024-01-01 12:00
  newData.payed = false;

  if (frame.can_dlc > Data::ID_SIZE) {
    // Clamp the length to prevent reading past the end of frame.data (max 8
    // bytes)
    size_t maxPlateInFrame = 8 - Data::ID_SIZE;
    size_t plateLenInFrame =
        std::min((size_t)(frame.can_dlc - Data::ID_SIZE), maxPlateInFrame);
    size_t plateLen = std::min(plateLenInFrame, Data::LICENCE_PLATE_SIZE - 1);

    std::copy(frame.data + Data::ID_SIZE, frame.data + Data::ID_SIZE + plateLen,
              newData.licenceplate.begin());
    newData.licenceplate[plateLen] = '\0';
  }

  _db.add(id, newData);
  Serial.printf("[CAN DB] Registered: ID %u\n", id);
}

void CANDatabaseHandler::handleValidation(const struct can_frame &frame) {
  if (frame.can_dlc < Data::ID_SIZE)
    return;

  uint16_t id = 0;
  memcpy(&id, frame.data, Data::ID_SIZE);

  auto session = _db.get(id);
  if (session) {
    session->payed = true;
    _db.add(id, *session);
    Serial.printf("[CAN DB] Validation: ID %u marked as PAID\n", id);
  } else {
    Serial.printf("[CAN DB] Validation Error: ID %u not found\n", id);
  }
}

void CANDatabaseHandler::handleSessionRequest(const struct can_frame &frame) {
  if (frame.can_dlc < Data::ID_SIZE)
    return;

  uint16_t id = 0;
  memcpy(&id, frame.data, Data::ID_SIZE);

  if (id == SpecialID::LOWEST_ID) {
    id = _db.getLowestId();
    Serial.printf("[CAN DB] Lowest ID requested, assigned: %u\n", id);

    struct can_frame response{};
    response.can_id = TX_SESSION_DATA;
    response.can_dlc = 3;
    response.data[0] = 0xFE; // index for free id instead of empty data
    memcpy(&response.data[1], &id, 2);
    _mcp.sendMessage(&response);
    return;
  }

  sendSessionData(id);
}

void CANDatabaseHandler::sendSessionData(uint16_t id) {
  auto session = _db.get(id);
  if (!session) {
    struct can_frame response{};
    response.can_id = TX_SESSION_DATA;
    response.can_dlc = 2;
    response.data[0] = 0xFF; // Error marker or Index 0xFF
    response.data[1] = static_cast<uint8_t>(SessionStatus::NOT_FOUND);
    _mcp.sendMessage(&response);
    Serial.printf("[CAN DB] Sending Session Data: ID %u, Status: NOT_FOUND\n",
                  id);
    return;
  }

  uint8_t status = session->payed
                       ? static_cast<uint8_t>(SessionStatus::PAID)
                       : static_cast<uint8_t>(SessionStatus::NOT_PAID);

  // Frame 0: [Index 0, Status, Timestamp (4 bytes), Plate[0..1]]
  struct can_frame f0{};
  f0.can_id = TX_SESSION_DATA;
  f0.can_dlc = 8;
  f0.data[0] = 0x00; // Index 0
  f0.data[1] = status;
  std::copy(reinterpret_cast<uint8_t *>(&session->created_at),
            reinterpret_cast<uint8_t *>(&session->created_at) + 4, &f0.data[2]);
  f0.data[6] = session->licenceplate[0];
  f0.data[7] = session->licenceplate[1];
  _mcp.sendMessage(&f0);
  delay(10);

  // Frame 1: [Index 1, Plate[2..8]]
  struct can_frame f1{};
  f1.can_id = TX_SESSION_DATA;
  f1.can_dlc = 8;
  f1.data[0] = 0x01; // Index 1
  std::copy(session->licenceplate.begin() + 2,
            session->licenceplate.begin() + 9, &f1.data[1]);
  _mcp.sendMessage(&f1);
  delay(10);

  // Frame 2: [Index 2, Plate[9..11]]
  struct can_frame f2{};
  f2.can_id = TX_SESSION_DATA;
  f2.can_dlc = 4;
  f2.data[0] = 0x02; // Index 2
  std::copy(session->licenceplate.begin() + 9,
            session->licenceplate.begin() + 12, &f2.data[1]);
  _mcp.sendMessage(&f2);

  Serial.printf("[CAN DB] Sent Session Data (3 frames) for ID %u\n", id);
}
