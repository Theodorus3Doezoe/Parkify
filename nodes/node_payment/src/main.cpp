#include "can_config.h"
#include "math.h"

#include <map>
#include "node_payment/include/Headers/RFIDScanner.h"
#include "../lib/common/ILicensePlateScanner.h"
#include "node_payment/include/Headers/PaymentController.h"

#define SCK 18
#define SIMO 19
#define MOSI 23



struct can_frame txMsg;
struct can_frame rxMsg;

MCP2515 mcp2515(CAN_SPI_CS_PIN);
MFRC522 rfid(RFID_SS, RFID_RST);

std::map<int, std::string> cars;
int carCount = 0;

std::string plate[] = {"C431WD", "DF54WD", "HJ078T", "VBNH31", "15TFHE"};

ILicensePlateScanner* scanner;
PaymentController* paymentController;

void setup() {
  Serial.begin(115200);
  SPI.begin(SCK, SIMO, MOSI);

  pinMode(CAN_SPI_CS_PIN, OUTPUT);
  pinMode(RFID_SS, OUTPUT);
  pinMode(RFID_RST, OUTPUT);

  digitalWrite(CAN_SPI_CS_PIN, HIGH);
  digitalWrite(RFID_SS, HIGH);
  digitalWrite(RFID_RST, HIGH);
  
  // Initialize CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);
  mcp2515.setNormalMode();
  
  // Initialize RFID reader
  scanner = new RFIDScanner(&rfid);
  rfid.PCD_Init();
  delay(100);
  
  digitalWrite(CAN_SPI_CS_PIN, HIGH);

  paymentController = new PaymentController(scanner, &mcp2515);
}

void loop() {

  paymentController->Run();

  delay(50);
}
