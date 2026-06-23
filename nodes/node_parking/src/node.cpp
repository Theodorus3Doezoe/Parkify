#include "include/node.h"

node::node(const std::array<spot, SPOTS>& inSpots, ICommunication* inCom) 
: spots(inSpots)
, com(inCom){}

void node::run()
{
    message_frame msg;
    msg.id = 0;
    msg.data[0] = 0;
    msg.data[1] = 0;
    bool received = com->rx_message(&msg);

    if (received) {
        Serial.printf("NODE: Receiving message, ID = %d\n", msg.id);

        if (msg.id == BR_ENTRY_NOTICE) {
            Rx_Entry();
        } else if (msg.id == TX_SPOT_INFO) {
            Rx_SpotInfo(&msg);
        } else if (msg.id > 20) {
            Rx_Prio(&msg);
        }
    }

    for (uint8_t i = 0; i < SPOTS; i++) {
        if (spots[i].update()) {
            Tx_SpotInfo(spots[i].getId(), spots[i].getOccupancy());
        }
    }

    updateReservations();
}

void node::updateReservations()
{
  if (reservationState == reservationSystemState::negotiating) {
    if ((millis() - lastInput) > NEGOTIATION_TIME) {
        reservationState = reservationSystemState::done;
        Serial.printf("NODE: Negotiation over at %d, last input at %d\n", millis(), lastInput);
    }
  } else if (reservationState == reservationSystemState::done) {
    for (uint8_t i = 0; i < SPOTS; i++) {
        if (spots[i].getState() == spotState::Reserved) {
            spots[i].setReservation(true, (spots[i].getReservationLevel() + 1));
        }
    }
    if (lowestNodePriority == ownPriority) {
      for (uint8_t i = 0; i < SPOTS; i++) {
        if (spots[i].getPriority() == lowestNodePriority) {
            Serial.printf("NODE: Won!, reserving spot %d\n", spots[i].getId());
            spots[i].setReservation(true, 1);
        }
      }
    }

    reservationState = reservationSystemState::idle;
    lowestNodePriority = 0xFFFF;
    lastInput = 0xFFFF;
  }
}

void node::Tx_Prio(uint16_t prio)
{
    message_frame msg {
        .id = (20 + ID),
        .data_length = 2
    };
    msg.data[0] = (prio & 0b11111111);
    msg.data[1] = ((prio >> 8) & 0b11111111);

    Serial.printf("NODE: Sending priority: %d on id %d\n", prio, msg.id);

    com->tx_message(msg);
}

void node::Tx_SpotInfo(uint16_t SpotId, bool occupancy)
{
    message_frame msg {
        .id = TX_SPOT_INFO,
        .data_length = 2
    };
    msg.data[0] = ((occupancy << 7) | (SpotId >> 8));
    msg.data[1] = (SpotId & 0b11111111);

    Serial.printf("NODE: Sending spot info: ID = %d Occupancy = %s\n", SpotId, (occupancy) ? "True" : "False");

    com->tx_message(msg);
}

void node::Rx_Entry()
{
    Serial.println("NODE: Entry annoucement received");
    reservationState = reservationSystemState::negotiating;

    spotInfo spot = getBestSpot();
    lowestNodePriority = spot.prio;
    if (spot.id > 0) {
        Serial.printf("NODE: Transmiting best spot: ID = %d, Prio = %d\n", spot.id, spot.prio);
        Tx_Prio(spot.prio);
        lastInput = millis();
    } else {
        Serial.println("NODE: No spots avalible");
    }
}

void node::Rx_Prio(message_frame* msg)
{
    if (reservationState == reservationSystemState::negotiating) {
        uint16_t prio;
        prio |= msg->data[0];
        prio |= (msg->data[1] << 8);

        Serial.printf("Receiving priority %d\n", prio);

        if (prio < lowestNodePriority) lowestNodePriority = prio;

        lastInput = millis();
    }

}

void node::Rx_SpotInfo(message_frame* msg)
{
    if (!(msg->data[0] >> 7)) return;

    for (uint8_t i = 0; i < SPOTS; i++) {
        if (spots[i].getState() == spotState::Reserved) {
            if (spots[i].getReservationLevel() > 1) {
                spots[i].setReservation(true, (spots[i].getReservationLevel() - 1));
            } else {
                spots[i].setReservation(false, 0);
            }
        }
    }
}

spotInfo node::getBestSpot()
{
    spotInfo spot {
        .id = 0,
        .prio = 0
    };
    uint16_t lowestPriority = 0xFFFF;

    for (uint8_t i = 0; i < SPOTS; i++) {
        if (spots[i].getPriority() < lowestPriority) {
            if (spots[i].getState() == spotState::Free) {
                lowestPriority = spots[i].getPriority();
                spot.prio = lowestPriority;
                spot.id = spots[i].getId();
            }
        }
    }

    ownPriority = lowestPriority;

    return spot;
}