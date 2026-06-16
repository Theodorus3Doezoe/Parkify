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
    if (license == "")
    {
        return;
    }

    struct can_frame txMsg;

    int length = license.length();
    char arr[length + 1];
    strcpy(arr, license.c_str());

    txMsg.can_id = TX_VALIDATED_REG;
    txMsg.can_dlc = strlen(arr);

    canBus->sendMessage(&txMsg);
}