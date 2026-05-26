#ifndef AVAILABILITY_MANAGER_H
#define AVAILABILITY_MANAGER_H

#include "interfaces/IDisplay.h"
#include "interfaces/IBus.h"

class AvailabilityManager {
private:
    IDisplay& oled;
    IBus& canBus;
    int totalSpots;
    int currentOccupied;

public:
    AvailabilityManager(IDisplay& display, IBus& bus, int capacity);
    void setup();
    void run();
};

#endif