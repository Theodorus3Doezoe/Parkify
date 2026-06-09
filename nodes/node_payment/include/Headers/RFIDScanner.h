#include "../lib/common/ILicensePlateScanner.h"
#include <MFRC522.h>


class RFIDScanner : public ILicensePlateScanner{

    private:
    MFRC522* rfid;
    
    public:
    RFIDScanner(MFRC522* rfid);
    String Scan() override;
};