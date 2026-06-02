#pragma once
#include <Arduino.h>
#include "CanCommunication.hpp"
#include "IMotor.hpp"
#include "ILicencePlateScanner.hpp"
#include "can_comms_protocol.h"
#include "can_config.h"

class IMotor;
class ILicencePlateScanner;

enum GateMode {
    ENTRY,
    EXIT
};

extern SystemState currentState;

extern int validatedParkingID;
extern String validatedReg;

class GateController {
public:
    GateController(CanCommunication &can, IMotor &motor, ILicencePlateScanner &scanner, GateMode mode);

    void begin();
    void loop();

    void processCanMessage(struct can_frame &frame);

private:

    CanCommunication &can;
    IMotor &motor;
    ILicencePlateScanner &scanner;
    GateMode mode;

    void handleOccupancy(struct can_frame &frame);
    void handleState(struct can_frame &frame);
    void handleValidation(struct can_frame &frame);
    void handleValidatedReg(struct can_frame &frame);

    void sendOccupancyRequest();
    void sendEntryNotice(int parkingID);
    void sendRegistration(String reg);
    void sendExitBroadcast(int parkingID);

    void entrySimulation();
    void exitDebug();
};