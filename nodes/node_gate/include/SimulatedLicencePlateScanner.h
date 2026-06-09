#ifndef SIMULATEDLICENCEPLATESCANNER_H
#define SIMULATEDLICENCEPLATESCANNER_H

#include "ILicencePlateScanner.h"

class SimulatedLicencePlateScanner : public ILicencePlateScanner 
{
    public:
        String scan() override;
};

#endif