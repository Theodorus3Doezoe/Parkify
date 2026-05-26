#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include <map>
#include <string>
#include "can_config.h"
#include "math.h"
#include <MFRC522.h>

using namespace std;

#define RFID_SS 21
#define RFID_RST 22


struct can_frame txMsg;
struct can_frame rxMsg;


MCP2515 mcp2515(CAN_SPI_CS_PIN);
MFRC522 rfid(RFID_SS, RFID_RST); 


std::map<int, pair<string, bool>> cars;
int carCount = 0;

string plate[] = {"C431WD","DF54WD","HJ078T","VBNH31","15TFHE"};

void generateMap();
void readRFIDCard();
String getCardUID();

void setup() {

  Serial.begin(115200);
  Serial.println("SETUP!");
  SPI.begin();
  
  // Initialize CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);
  mcp2515.setLoopbackMode();
  
  // Initialize RFID reader
  rfid.PCD_Init();
  delay(100);
  
  Serial.println("RFID reader initialized");
  //rfid.PCD_DumpVersionToSerial();
  
  //generateMap();

  const char* text = "MDKF43";
  //readRFIDCard();
  
  txMsg.can_id = 4;
  txMsg.can_dlc = strlen(text);

  memcpy(txMsg.data, text, txMsg.can_dlc);

  mcp2515.sendMessage(&txMsg);
  Serial.println("SETUP!");
}

void loop() {
  char received[9];
  if(mcp2515.readMessage(&rxMsg) == MCP2515::ERROR_OK)
  {
    Serial.println("LOOP!");
    Serial.println("Received");
    if(rxMsg.can_id == 4)
    {
      char received[9];
      Serial.println("LOOP!");
      memcpy(received, rxMsg.data, rxMsg.can_dlc);
      received[rxMsg.can_dlc] = '\0';

      Serial.print("Received: ");
      Serial.println(received);
    }
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        Serial.println("RFID tag detected!");

      for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      rfid.PICC_HaltA();
    }
}
void readRFIDCard() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Check if we can read the card serial
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  // Get the card UID
  String cardUID = getCardUID();
  Serial.print("Card UID: ");
  Serial.println(cardUID);
  
  // Halt PICC (end the card reading)
  rfid.PICC_HaltA();
  
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

String getCardUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uid += "0";
    }
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

void generateMap()
{
  Serial.println("GENERATE MAP!");
  for (int i = 0; i <= 4; i++)
  {
    cars.insert({i, {plate[i], false}});
    carCount++;
  }
}