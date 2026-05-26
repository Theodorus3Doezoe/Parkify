#include "IStatusIndicator.hpp"
#include "IOccupancySensor.hpp"
#include "ICommunication.hpp"

class mockIndicator : public IStatusIndicator {
private:
    indicatorState mockState;

public:
    mockIndicator();
    void setLightState(indicatorState state) override;
};

class mockSensor : public IOccupancySensor {
private:
    bool mockState;

public:
    mockSensor(bool state);
    bool sensorTriggered() override;
    void setOccupancy(bool occupied);
};

class mockCommunication : public ICommunication {
private:
    message_frame stored_msg;
public:
    mockCommunication();
    void set_message(uint8_t id, uint8_t dlc, uint8_t data[8]);
    bool tx_message(message_frame msg) override;
    message_frame rx_message() override;
};