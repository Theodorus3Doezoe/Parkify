#include "mock.hpp"

mockIndicator::mockIndicator() {}

void mockIndicator::setLightState(indicatorState state) { mockState = state; }


mockSensor::mockSensor(bool state) : mockState(state) {}

bool mockSensor::sensorTriggered() { return mockState; }

void mockSensor::setOccupancy(bool occupied) { mockState = occupied; }


mockCommunication::mockCommunication() {}

void mockCommunication::set_message(uint8_t id, uint8_t dlc, uint8_t data[8])
{
    stored_msg.id = id;
    stored_msg.data_length = dlc;

    for (uint8_t i = 0; i < dlc; i++) {
        stored_msg.data[i] = data[i];
    }
}

bool mockCommunication::tx_message(message_frame msg) { return true; }

message_frame mockCommunication::rx_message()
{
    message_frame msg = stored_msg;

    stored_msg.id = 0;
    stored_msg.data_length = 0;
    
    for (uint8_t i = 0; i < 8; i++) {
        stored_msg.data[i] = 0;
    }

    return stored_msg;
}