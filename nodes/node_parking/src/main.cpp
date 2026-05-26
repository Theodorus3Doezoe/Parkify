#include <Arduino.h>
#include <SPI.h>
#include "includes/mcp2515.h"
#include "spot.hpp"
#include "interfaces/mock.hpp"
#include "interfaces/canbus.hpp"


#define SPOT_COUNT 4; // Amount of spots managed by this node
#define SPOT_IDS { 1, 2, 3, 4 } // IDs for each of the spots
#define SPOT_PRIOS { 1, 2, 3, 4 } // Priority values for each spot

canbus com(5);

uint16_t spotId[4] = SPOT_IDS;
uint16_t spotPrio[4] = SPOT_PRIOS;
mockIndicator indicators[4];
mockSensor sensors[4] = {
  mockSensor(false),
  mockSensor(false),
  mockSensor(false),
  mockSensor(false)
};

spot spots[4] = {
  spot(spotId[0], spotPrio[0], &sensors[0], &indicators[0]),
  spot(spotId[1], spotPrio[1], &sensors[1], &indicators[1]),
  spot(spotId[2], spotPrio[2], &sensors[2], &indicators[2]),
  spot(spotId[3], spotPrio[3], &sensors[3], &indicators[3])
};

uint16_t spotUpdate;

enum reservationSystemState { idle, negotiating, done };

reservationSystemState reservationState = reservationSystemState::idle;
uint16_t lastInput = 0xFFFF;
uint16_t lowestNodePriority = 0xFFFF;
uint16_t ownPriority = 0xFFFF;


void Tx_Prio(uint16_t prio)
{
  message_frame msg {
    .id = 3,
    .data_length = 2
  };
  msg.data[0] = (prio & 0b11111111);
  msg.data[1] = ((prio >> 8) & 0b11111111);

  com.tx_message(msg);
}


void Tx_SpotInfo(uint16_t SpotId, bool occupancy)
{
  message_frame msg {
    .id = 6,
    .data_length = 2
  };
  msg.data[0] = ((occupancy << 7) | (SpotId >> 8));
  msg.data[1] = (SpotId & 0b11111111);
	
	com.tx_message(msg);
}


void Rx_Prio(__u8 data[8])
{
  uint16_t prio;
  prio =+ data[0];
  prio =+ (data[1] << 8);

  if (prio < lowestNodePriority) lowestNodePriority = prio;

  lastInput = millis();
}

void updateReservation()
{
  if (reservationState == reservationSystemState::negotiating) {
    if ((millis() - lastInput) > 500) reservationState = reservationSystemState::done ;
  } else if (reservationState == reservationSystemState::done) {
    if (lowestNodePriority == ownPriority) {
      for (uint8_t i = 0; i < 4; i++) {
        if (spots[i].getPriority() == lowestNodePriority) {
          spots[i].setReservation(true);
        }
      }
    }

    reservationState = reservationSystemState::idle;
    lowestNodePriority = 0xFFFF;
    lastInput = 0xFFFF;
  }
}


uint16_t lowestPrio()
{
  uint16_t lowestPriority = 0xFFFF;

  for (uint8_t i = 0; i < 4; i++) {
    if (spots[i].getPriority() < lowestPriority) {
      if (spots[i].getOccupancy()) lowestPriority = spots[i].getPriority();
    }
  }

  ownPriority = lowestPriority;

  return lowestPriority;
}


void userInput()
{
    if (Serial.available() > 0) {
    int input = Serial.read() - 48;
    uint16_t spotIndex = 0;

    for (uint8_t i = 0; i < 4; i++) {
      if (input == spots[i].getId()) spotIndex = i;
    }

    bool occupancy = !spots[spotIndex].getOccupancy();

    sensors[spotIndex].setOccupancy(occupancy);
    Tx_SpotInfo(spots[spotIndex].getId(), occupancy);

    Serial.print("Spot: ");
    Serial.print(spots[spotIndex].getId());
    if (occupancy) {
      Serial.println(" is: occupied");
    } else {
      Serial.println(" is: free ");
    }
  }
}


void setup()
{
  Serial.begin(115200);
}


void loop()
{
  userInput(); // Should only be used with mock sensors

  message_frame msg;
  msg.id = 0;

  msg = com.rx_message();

  if (msg.id > 0) {
    if (msg.id == 3) {
      Serial.print("Entry detected, finding best availible spot: ");
      uint16_t lowestPriority = lowestPrio();
      if (lowestPriority < 0xFFFF) {
        Serial.println(lowestPrio());
        Tx_Prio(lowestPrio());
        reservationState = reservationSystemState::negotiating;
      } else {
        Serial.println("No spots availible");
      }
    } else if (msg.id > 10) {
      Serial.print("Node priority received: ");
      for(uint8_t i = 0; i < sizeof(msg.data); i++) {
        Serial.print(msg.data[i]);
      }
      Serial.println("");
      if (reservationState == reservationSystemState::negotiating) {
        Rx_Prio(msg.data);
      }
    }
  }

  updateReservation();

  if (spotUpdate > (millis() + 100)) {
    spotUpdate = millis();
    for (uint8_t i = 0; i < 4; i++) {
      spots[i].update();
    }
  }
}