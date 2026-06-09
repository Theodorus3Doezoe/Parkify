#include "node_payment/include/Headers/RFIDScanner.h"
#include <Arduino.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

String lastLicense;

RFIDScanner::RFIDScanner(MFRC522* rfid) : rfid(rfid)
{
}

String RFIDScanner::Scan()
{
    if (rfid->PICC_IsNewCardPresent() && rfid->PICC_ReadCardSerial()){
        if (rfid->uid.size > 0) {
            String license = String(rfid->uid.uidByte[rfid->uid.size - 3] + rfid->uid.uidByte[rfid->uid.size - 2] + rfid->uid.size - 1);
            
            if(license != lastLicense)
            {
                lastLicense = license;
                Serial.println(license);
                return license;
            }
        }
    }
    return "";
}