#include "can_db_handler.h"
#include "can_comms_protocol.h"
#include "node_db/include/can_db_handler.h"
#include <Arduino.h>
#include <cstdint>

CANDatabaseHandler::CANDatabaseHandler(MCP2515 &mcp, Database &db)
    : _mcp(mcp), _db(db) {}

void CANDatabaseHandler::checkMessages() {
  struct can_frame frame;
  while (_mcp.readMessage(&frame) == MCP2515::ERROR_OK) {
    switch (frame.can_id) {
    case BR_ENTRY_NOTICE:
      handleEntryNotice(frame);
      break;
    case TX_ENTRY_REGISTRATION:
      handleEntryRegistration(frame);
      break;
    case TX_VALIDATION:
      handleValidation(frame);
      break;
    case REQ_SESSION_DATA:
      handleSessionRequest(frame);
      break;
    case BR_EXIT:
      handleExit(frame);
      break;

    case TX_SESSION_DATA:
      // For loopback testing/diagnostics
      {
        uint8_t firstByte = frame.data[0];
        if (firstByte == 0xFF) {
          Serial.println("[CAN DB] LOOPBACK: Session NOT FOUND");
        } else if (firstByte == 0xFE) {
          uint16_t assignedId = 0;
          memcpy(&assignedId, &frame.data[1], 2);
          Serial.printf("[CAN DB] LOOPBACK: Assigned ID received: %u\n",
                        assignedId);
        } else {
          // New 1-frame format: [Status, Timestamp(4), Plate(3)]
          uint8_t status = frame.data[0];
          uint32_t timestamp = 0;
          memcpy(&timestamp, &frame.data[1], 4);
          char plate[4] = {0};
          memcpy(plate, &frame.data[5], 3);
          Serial.printf("[CAN DB] LOOPBACK: Session Data - Status: %u, Time: "
                        "%u, Plate: %s\n",
                        status, timestamp, plate);
        }
      }
      break;
    default:

      break;
    }
  }
}

void CANDatabaseHandler::handleEntryNotice(const struct can_frame &frame) {
  if (frame.can_dlc < 2)
    return;
  memcpy(&_lastNoticedId, frame.data, 2);
  Serial.printf("[CAN DB] Notice: ID %u\n", _lastNoticedId);
}

void CANDatabaseHandler::handleEntryRegistration(
    const struct can_frame &frame) {
  Data newData{};
  newData.created_at = (uint32_t)time(NULL);
  newData.payed = false;

  size_t plateLen =
      std::min((size_t)frame.can_dlc, Data::LICENCE_PLATE_SIZE - 1);
  memcpy(newData.licenceplate.data(), frame.data, plateLen);
  newData.licenceplate[plateLen] = '\0';

  _db.add(_lastNoticedId, newData);
  Serial.printf("[CAN DB] Reg: ID %u, Plate: %s\n", _lastNoticedId,
                newData.licenceplate.data());
}

void CANDatabaseHandler::handleValidation(const struct can_frame &frame) {
  if (frame.can_dlc < Data::ID_SIZE) return;
  uint16_t id = 0;
  memcpy(&id, frame.data, Data::ID_SIZE);

  auto session = _db.get(id);
  if (session) {
    session->payed = true;
    _db.add(id, *session);
    Serial.printf("[CAN DB] Paid: ID %u\n", id);

    struct can_frame response{};
    response.can_id = TX_VALIDATED_REG;
    response.can_dlc = 2 + 3; // ID (2) + Plate (3)
    memcpy(response.data, &id, 2);
    memcpy(response.data + 2, session->licenceplate.data(), 3);
    _mcp.sendMessage(&response);
  } else {
    Serial.printf("[CAN DB] Error: ID %u not found\n", id);
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

void CANDatabaseHandler::handleIdByPlateRequest(const struct can_frame &frame) {
  if (frame.can_dlc < 3)
    return;

  char plate[4] = {0};
  memcpy(plate, frame.data, 3);

  auto id = _db.findIdByPlate(plate);

  struct can_frame response{};
  response.can_id = TX_SESSION_DATA;

  if (id) {
    Serial.printf("[CAN DB] Plate '%s' found, ID: %u\n", plate, *id);
    response.can_dlc = 3;
    response.data[0] = 0xFE;
    memcpy(&response.data[1], &(*id), 2);
  } else {
    Serial.printf("[CAN DB] Plate '%s' NOT found\n", plate);
    response.can_dlc = 1;
    response.data[0] = 0xFF;
  }
  _mcp.sendMessage(&response);
}

void CANDatabaseHandler::sendSessionData(uint16_t id) {
  auto session = _db.get(id);
  struct can_frame response{};
  response.can_id = TX_SESSION_DATA;

  if (!session) {
    response.can_dlc = 2;
    response.data[0] = 0xFF; // Error marker
    response.data[1] = static_cast<uint8_t>(SessionStatus::NOT_FOUND);
    _mcp.sendMessage(&response);
    Serial.printf("[CAN DB] Not Found: ID %u\n", id);
    return;
  }

  uint8_t status = session->payed
                       ? static_cast<uint8_t>(SessionStatus::PAID)
                       : static_cast<uint8_t>(SessionStatus::NOT_PAID);

  response.can_dlc = 8;
  response.data[0] = status;
  memcpy(&response.data[1], &session->created_at, 4);
  memcpy(&response.data[5], session->licenceplate.data(), 3);

  _mcp.sendMessage(&response);
  Serial.printf("[CAN DB] Sent Data: ID %u\n", id);
}

void CANDatabaseHandler::handleExit(const struct can_frame &frame) {
  if (frame.can_dlc < Data::ID_SIZE)
    return;
  uint16_t id = 0;
  memcpy(&id, frame.data, Data::ID_SIZE);

  if (_db.remove(id)) {
    Serial.printf("[CAN DB] Removed: ID %u\n", id);
  } else {
    Serial.printf("[CAN DB] Exit error: ID %u not found\n", id);
  }
}
