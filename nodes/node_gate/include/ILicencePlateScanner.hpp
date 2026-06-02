#ifndef ILICENCEPLATESCANNER_H
#define ILICENCEPLATESCANNER_H

#include "GateController.hpp"

class ILicencePlateScanner
{
    public:
        virtual ~ILicencePlateScanner() = default;

        virtual String scan() = 0;
};

#endif