#pragma once
#include <Arduino.h>
#include "CanCommunication.h"
#include "IMotor.h"
#include "ILicencePlateScanner.h"
#include "can_comms_protocol.h"
#include "can_config.h"
#include <mcp2515.h>
#include <MFRC522.h>
#include <mutex>
#include <condition_variable>

class IMotor;
class ILicencePlateScanner;

enum GateMode {
    ENTRY,
    EXIT
};

extern SystemState currentState;

extern MCP2515 mcp2515;
extern MFRC522 rfid;

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
    void handleParkingID(struct can_frame &frame);

    bool waitForParkingID(int timeout_ms);

    void sendOccupancyRequest();
    void sendEntryNotice(int parkingID);
    void sendRegistration(String reg);
    void sendExitBroadcast(int parkingID);

    void entrySimulation();
    void exitDebug();

    uint16_t parkingID;

    std::mutex mtx;
    std::condition_variable cv;

    bool parkingID_ready = false;
};