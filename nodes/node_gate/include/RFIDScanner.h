#ifndef RFIDSCANNER_H
#define RFIDSCANNER_H

#include "ILicencePlateScanner.h"
#include <MFRC522.h>


class RFIDScanner : public ILicencePlateScanner{

    private:
    MFRC522* rfid;
    
    public:
    RFIDScanner(MFRC522* rfid);
    String scan() override;
    void begin();
};

#endif