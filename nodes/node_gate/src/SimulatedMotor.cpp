#include "../include/SimulatedMotor.h"

void SimulatedMotor::open()
{
    Serial.println("Gate opened.");
};

void SimulatedMotor::close()
{
    Serial.println("Gate closed.");
};