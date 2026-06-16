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


    // Sending parking ID over CAN
    u_int16_t parkingID = dbClient.requestIdByPlate(license.c_str());
    length = sizeof(parkingID);
    // copy raw bytes of parkingID into the CAN frame data
    txMsg.can_id = TX_VALIDATION;
    txMsg.can_dlc = length;
    // can_frame.data is an array, copy bytes into it
    memcpy(txMsg.data, &parkingID, length);
    canBus->sendMessage(&txMsg);




    // Sending licence plate over CAN
    length = license.length();
    if (length > 8) length = 8; // ensure fits in CAN data field
    txMsg.can_id = TX_VALIDATED_REG;
    txMsg.can_dlc = length;
    memcpy(txMsg.data, license.c_str(), length);
    canBus->sendMessage(&txMsg);

    delay(10);

    if(!dbClient.isPaid(parkingID));
    {
        dbClient.setSessionPaid(parkingID);
    }
}