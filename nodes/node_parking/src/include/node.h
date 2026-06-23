#include "spot.h"
#include "ICommunication.h"
#include "definitions.h"
#include "can_comms_protocol.h"
#include <iostream>
#include <array>

enum reservationSystemState { idle, negotiating, done };

typedef struct {
    uint16_t id;
    uint16_t prio;
} spotInfo;

class node {
private:
    std::array<spot, SPOTS> spots;
    ICommunication* com;
    reservationSystemState reservationState = reservationSystemState::idle;
    uint32_t lastInput = 0;
    uint16_t lowestNodePriority = 0xFFFF;
    uint16_t ownPriority = 0xFFFF;
    
    void updateReservations();
    void Tx_Prio(uint16_t prio);
    void Tx_SpotInfo(uint16_t SpotId, bool occupancy);
    void Rx_Entry();
    void Rx_Prio(message_frame* msg);
    void Rx_SpotInfo(message_frame* msg);
    spotInfo getBestSpot();

public:
    node(const std::array<spot, SPOTS>& inSpots, ICommunication* inCom);
    void run();
};