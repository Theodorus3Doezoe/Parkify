#include "../include/SimulatedMotor.hpp"

void SimulatedMotor::open()
{
    Serial.println("Gate opened.");
};

void SimulatedMotor::close()
{
    Serial.println("Gate closed.");
};