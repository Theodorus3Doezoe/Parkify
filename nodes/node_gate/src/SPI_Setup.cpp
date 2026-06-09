#include "../include/SPI_Setup.h"
#include "../include/CanCommunication.h"
#include "../include/RFIDScanner.h"
#include "../include/GateController.h"

#include <SPI.h>
#include <mcp2515.h>
#include <MFRC522.h>

// SPI pins
#define CAN_SCK  18
#define CAN_MISO 19
#define CAN_MOSI 23

#define CAN_CS   5

// external objects (assumed defined somewhere)
extern MCP2515 mcp2515;
extern MFRC522 rfid;

void SPI_Setup::begin(GateMode gateMode)
{
    SPI.begin(CAN_SCK, CAN_MISO, CAN_MOSI);

    pinMode(CAN_CS, OUTPUT);
    pinMode(RFID_SS, OUTPUT);
    pinMode(RFID_RST, OUTPUT);

    digitalWrite(CAN_CS, HIGH);
    digitalWrite(RFID_SS, HIGH);
    digitalWrite(RFID_RST, HIGH);

    delay(50);

    // ---- INIT CAN ----
    mcp2515.reset();
    mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);
    mcp2515.setNormalMode();

    Serial.println("[SYSTEM] CAN initialized");

    delay(50);

    if (gateMode == ENTRY) {
        // ---- ENSURE BUS IS FREE ----
        digitalWrite(CAN_CS, HIGH);

        // ---- INIT RFID (IMPORTANT FIX HERE) ----
        rfid.PCD_Init(RFID_SS, RFID_RST);
        byte v = rfid.PCD_ReadRegister(rfid.VersionReg);
        rfid.PCD_AntennaOn();

        Serial.print("[RFID] Version: ");
        Serial.println(v, HEX);

        delay(50);

        Serial.println("[SYSTEM] RFID initialized (ENTRY mode)");
    } else {
        Serial.println("[SYSTEM] EXIT mode → RFID disabled");
    }

    Serial.println("[SYSTEM] SPI for CAN + RFID ready");
}