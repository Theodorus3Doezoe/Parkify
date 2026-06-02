#ifndef SIMULATEDMOTOR_H
#define SIMULATEDMOTOR_H

#include "IMotor.hpp"

class SimulatedMotor : public IMotor
{
    public:
        void open() override;
        void close() override;
};

#endif