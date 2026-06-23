#include "IStatusIndicator.h"
#include "IOccupancySensor.h"
#include "ICommunication.h"

class mockIndicator : public IStatusIndicator {
private:
    indicatorState mockState;

public:
    mockIndicator();
    void setLightState(indicatorState state) override;
    void update() override;
};

class mockSensor : public IOccupancySensor {
private:
    sensorState mockState;

public:
    mockSensor(bool occupied);
    sensorState sensorTriggered() override;
    void setOccupancy(bool occupied);
};

class mockCommunication : public ICommunication {
private:
    message_frame stored_msg;
public:
    mockCommunication();
    void set_message(uint8_t id, uint8_t dlc, uint8_t data[8]);
    bool tx_message(message_frame msg) override;
    bool rx_message(message_frame* msg) override;
};