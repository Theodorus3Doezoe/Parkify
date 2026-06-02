#ifndef SIMULATEDLICENCEPLATESCANNER_H
#define SIMULATEDLICENCEPLATESCANNER_H

#include "ILicencePlateScanner.hpp"

class SimulatedLicencePlateScanner : public ILicencePlateScanner 
{
    public:
        String scan() override;
};

#endif