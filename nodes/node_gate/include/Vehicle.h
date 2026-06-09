#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>

class Vehicle
{
public:
    Vehicle(int parkingID, 
            const String& registration,
            bool isValidated = false)
    : parkingID(parkingID), registration(registration), isValidated(isValidated) {}

    ~Vehicle() = default;

private:
    int parkingID;
    String registration;
    bool isValidated = false;
};

#endif