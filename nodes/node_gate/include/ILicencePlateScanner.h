#ifndef ILICENCEPLATESCANNER_H
#define ILICENCEPLATESCANNER_H

#include "GateController.h"

#define RFID_SS 21
#define RFID_RST 22

class ILicencePlateScanner
{
    public:
        virtual ~ILicencePlateScanner() = default;

        virtual String scan() = 0;
};

#endif