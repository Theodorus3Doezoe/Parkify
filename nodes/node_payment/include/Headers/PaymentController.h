#ifndef PAYMENTCONTROLLER_H
#define PAYMENTCONTROLLER_h
#include "../lib/common/ILicensePlateScanner.h"
#include "../lib/common/can_config.h"
#include "../lib/common/can_comms_protocol.h"

class PaymentController{

    private:
    ILicensePlateScanner* scanner;
    MCP2515* canBus;

    public:
    PaymentController(ILicensePlateScanner* scanner, MCP2515* canBus);
    ~PaymentController();

    void Run();
};

#endif