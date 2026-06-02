#ifndef IMOTOR_H
#define IMOTOR_H

#include "GateController.hpp"

class IMotor 
{
    public:
        virtual ~IMotor() = default;

        virtual void open() = 0;
        virtual void close() = 0;
};

#endif