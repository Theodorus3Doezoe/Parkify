#include "node_payment/include/Headers/PaymentController.h"

#include <Arduino.h>



PaymentController::PaymentController(ILicensePlateScanner* scanner, MCP2515* canBus) 
: scanner(scanner)
, canBus(canBus)
, dbClient(*canBus)
{}

void PaymentController::Run()
{
    String license = scanner->Scan();
    int length;
    if (license == "")
    {
        return;
    }

    struct can_frame txMsg;
    u_int16_t parkingID = dbClient.requestIdByPlate(license.c_str());


    // Sending parking ID over CAN
    Serial.println(license.c_str());
    Serial.println(parkingID);
    length = sizeof(parkingID);
    // copy raw bytes of parkingID into the CAN frame data
    txMsg.can_id = TX_VALIDATION;
    txMsg.can_dlc = 2;
    txMsg.data[0] = parkingID;
    canBus->sendMessage(&txMsg);




    // Sending licence plate over CAN
    if (length > 8) length = 8; // ensure fits in CAN data field
    txMsg.can_id = TX_VALIDATED_REG;
    txMsg.can_dlc = 2;
    txMsg.data[0] = license.toInt();
    canBus->sendMessage(&txMsg);

    delay(10);

    if(!dbClient.isPaid(parkingID));
    {
        dbClient.setSessionPaid(parkingID);
    }
}