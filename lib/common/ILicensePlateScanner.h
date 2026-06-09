#ifndef ILICENSEPLATESCANNER_H
#define ILICENSEPLATESCANNER_H
#include <string>
#include <Arduino.h>
#define RFID_SS 21
#define RFID_RST 22

class ILicensePlateScanner{
    public:
    virtual String Scan() = 0;
};

#endif