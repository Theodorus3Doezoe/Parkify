#include <Arduino.h>
#include "include/spot.h"
#include "include/node.h"
#include "include/mock.h"
#include "include/sonarsensor.h"
#include "include/singleLedIndicator.h"
#include "include/canbus.h"
#include "include/definitions.h"

canbus com(5);

mockIndicator indicators[SPOTS];
mockSensor sensors[SPOTS] = {
  mockSensor(false),
  mockSensor(false),
  mockSensor(false),
  mockSensor(false)
};

sonarsensor sonar[2] = {
  sonarsensor(12, 14, SENSOR_TRIGGER_DISTANCE),
  sonarsensor(27, 26, SENSOR_TRIGGER_DISTANCE)
};

singleLedIndicator leds[4] = {
  singleLedIndicator(32),
  singleLedIndicator(15),
  singleLedIndicator(2),
  singleLedIndicator(4)
};

std::array<spot, SPOTS> spots = {
  spot(SPOT_IDS[0], SPOT_PRIOS[0], &sonar[0], &leds[0]),
  spot(SPOT_IDS[1], SPOT_PRIOS[1], &sonar[1], &leds[1]),
  spot(SPOT_IDS[2], SPOT_PRIOS[2], &sensors[2], &leds[2]),
  spot(SPOT_IDS[3], SPOT_PRIOS[3], &sensors[3], &leds[3])
};

node controller(spots, &com);

void userInput()
{
  if (Serial.available() > 0) {
    uint8_t input = Serial.read() - 47;
    if (input <= SPOTS) {
      sensors[input].setOccupancy(!(sensors[input].sensorTriggered() == sensorState::Occupied));
    }
  }
}


void setup()
{
  Serial.begin(115200);
}


void loop()
{
  controller.run();
  userInput();
}