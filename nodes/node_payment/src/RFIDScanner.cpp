#include "node_payment/include/Headers/RFIDScanner.h"
#include <Arduino.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

String lastLicence;

RFIDScanner::RFIDScanner(MFRC522* rfid) : rfid(rfid)
{
}

String RFIDScanner::Scan()
{
if (rfid->PICC_IsNewCardPresent() && rfid->PICC_ReadCardSerial()) {

        if (rfid->uid.size > 0) {

            // keep your working numeric conversion
            unsigned long value = 0;

            for (byte i = 0; i < rfid->uid.size; i++) {
                value = (value * 31) + rfid->uid.uidByte[i];
            }

            // convert to string
            String fullValue = String(value);

            // ensure we have at least 3 digits
            while (fullValue.length() < 3) {
                fullValue = "0" + fullValue;
            }

            // extract last 3 digits
            String licence = fullValue.substring(fullValue.length() - 3);

            if (licence != lastLicence) {
                lastLicence = licence;

                Serial.print("[RFID PLATE] ");
                Serial.println(licence);

                rfid->PICC_HaltA();
                rfid->PCD_StopCrypto1();

                return licence;
            }

            rfid->PICC_HaltA();
            rfid->PCD_StopCrypto1();
        }
    }

    return "";
}