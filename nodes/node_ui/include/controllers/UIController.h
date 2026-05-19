#pragma once

#include <mcp2515.h>
#include "interfaces/IInputReader.h"
#include "interfaces/IDisplay.h"
#include "interfaces/IStateController.h"
#include "interfaces/ISpotManager.h"
#include "interfaces/IGateController.h"
#include "models/SpotStatus.h"

class UIController {
private:
    MCP2515& _mcp;
    IInputReader& _inputReader;
    IDisplay& _display;
    IStateController& _stateController;
    ISpotManager& _spotManager;
    IGateController& _gateController;

    void handleInput(String input);
    void onSpotUpdate(int spotID, bool occupied);
    void onStateChange(int state);

public:
    UIController(MCP2515& mcp, IInputReader& ir, IDisplay& d, IStateController& sc, ISpotManager& sm, IGateController& gc);
    void run();
    void updateFromCAN();
};
