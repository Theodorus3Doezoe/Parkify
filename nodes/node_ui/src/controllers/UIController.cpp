#include "controllers/UIController.h"
#include "can_comms_protocol.h"
#include "can_config.h"

UIController::UIController(MCP2515& mcp, IInputReader& ir, IDisplay& d, IStateController& sc, ISpotManager& sm, IGateController& gc)
    : _mcp(mcp), _inputReader(ir), _display(d), _stateController(sc), _spotManager(sm), _gateController(gc) {
}

void UIController::run() {
    String input = _inputReader.readInput();
    if (input.length() > 0) {
        handleInput(input);
    }
    updateFromCAN();
}

void UIController::handleInput(String input) {
    input.trim();
    if (input.equalsIgnoreCase("O")) {
        onStateChange(SystemState::OPEN);
    } else if (input.equalsIgnoreCase("C")) {
        onStateChange(SystemState::CLOSED);
    } else if (input.equalsIgnoreCase("E")) {
        onStateChange(SystemState::EMERGENCY);
    }
}

void UIController::onSpotUpdate(int spotID, bool occupied) {
}

void UIController::onStateChange(int state) {
    _stateController.setState(state);
}

void UIController::updateFromCAN() {
    struct can_frame frame;
    if (_mcp.readMessage(&frame) == MCP2515::ERROR_OK) {
        switch (frame.can_id) {
            case TX_SPOT_INFO:
                // Logic from skeleton: print spot info
                Serial.printf("Spot %d: %s\n", (int)frame.data[1], frame.data[0] ? "Taken" : "Free");
                break;
            case BR_STATE:
                // Logic from skeleton: print if emergency
                if (frame.data[0] == SystemState::EMERGENCY) {
                    Serial.println("EMERGENCY!");
                }
                break;
        }
    }
}
