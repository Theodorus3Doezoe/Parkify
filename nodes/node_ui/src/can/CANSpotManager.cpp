#include "can/CANSpotManager.h"
#include <Arduino.h>

CANSpotManager::CANSpotManager(MCP2515& mcp) : _mcp(mcp) {}

void CANSpotManager::reserveSpot(int spotID) {
}

void CANSpotManager::releaseSpot(int spotID) {
}
