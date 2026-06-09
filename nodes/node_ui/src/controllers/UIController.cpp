#include "controllers/UIController.h"
#include "can_comms_protocol.h"
#include "can_config.h"

UIController::UIController(MCP2515 &mcp, IInputReader &ir, IDisplay &d,
                           IStateController &sc, ISpotManager &sm,
                           IGateController &gc)
    : _mcp(mcp), _inputReader(ir), _display(d), _stateController(sc),
      _spotManager(sm), _gateController(gc) {}

void UIController::run() {
  String input = _inputReader.readInput();
  if (input.length() > 0) {
    handleInput(input);
  }
  updateFromCAN();
  _display.render();
  delay(50);
}

void UIController::handleInput(String input) {
  input.trim();
  if (input.equalsIgnoreCase("O")) {
    onStateChange(SystemState::OPEN);
    _display.logMessage("Set State: OPEN");
  } else if (input.equalsIgnoreCase("C")) {
    onStateChange(SystemState::CLOSED);
    _display.logMessage("Set State: CLOSED");
  } else if (input.equalsIgnoreCase("E")) {
    onStateChange(SystemState::EMERGENCY);
    _display.logMessage("Set State: EMERGENCY");
  } else if (input.equalsIgnoreCase("G")) {
    _gateController.openGate(1);
    _display.logMessage("Command: Open Entry Gate");
  } else if (input.equalsIgnoreCase("H")) {
    _gateController.closeGate(1);
    _display.logMessage("Command: Close Entry Gate");
  } else if (input.equalsIgnoreCase("J")) {
    _gateController.openGate(2);
    _display.logMessage("Command: Open Exit Gate");
  } else if (input.equalsIgnoreCase("K")) {
    _gateController.closeGate(2);
    _display.logMessage("Command: Close Exit Gate");
  }
}

void UIController::onSpotUpdate(int spotID, bool occupied) {
  SpotStatus status = {spotID, occupied};
  _display.showSpotOverview(&status, 1);
}

void UIController::onStateChange(int state) {
  _stateController.setState(state);
  _display.showState(state);
}

void UIController::updateFromCAN() {
  struct can_frame frame;
  static int vehicleCount = 0;

  if (_mcp.readMessage(&frame) == MCP2515::ERROR_OK) {
    switch (frame.can_id) {
    case TX_SPOT_INFO:
      onSpotUpdate((int)frame.data[1], (bool)frame.data[0]);
      _display.logMessage("Spot " + String((int)frame.data[1]) + " update");
      break;
    case BR_STATE:
      _display.showState(frame.data[0]);
      if (frame.data[0] == SystemState::EMERGENCY) {
        _display.logMessage("!!! EMERGENCY DETECTED !!!");
      } else {
        _display.logMessage("System state updated");
      }
      break;
    case BR_ENTRY_NOTICE:
      vehicleCount++;
      _display.showVehicleCount(vehicleCount);
      _display.logMessage("Vehicle Entered (+1)");
      break;
    case BR_EXIT:
      if (vehicleCount > 0)
        vehicleCount--;
      _display.showVehicleCount(vehicleCount);
      _display.logMessage("Vehicle Exited (-1)");
      break;
    }
  }
}
