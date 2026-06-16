#ifndef PAYMENTCONTROLLER_H
#define PAYMENTCONTROLLER_h
#include "../lib/common/ILicensePlateScanner.h"
#include "../lib/common/can_config.h"
#include "../lib/common/can_comms_protocol.h"
#include "../lib/common/database/database_client.h"

class PaymentController{

    private:
    ILicensePlateScanner* scanner;
    MCP2515* canBus;
    DatabaseClient dbClient;

    public:
    PaymentController(ILicensePlateScanner* scanner, MCP2515* canBus);
    ~PaymentController();

    void Run();
};

#endif