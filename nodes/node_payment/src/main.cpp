#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include <map>
#include <string>
#include "can_config.h"
#include "math.h"
#include <MFRC522.h>

#define RFID_SS 21
#define RFID_RST 22

using namespace std;

struct can_frame txMsg;
struct can_frame rxMsg;

MCP2515 mcp2515(CAN_SPI_CS_PIN);
MFRC522 rfid(RFID_SS, RFID_RST); 

std::map<int, string> cars;
int carCount = 0;

string plate[] = {"C431WD","DF54WD","HJ078T","VBNH31","15TFHE"};

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
  

  const char* text = "MDKF43";
  
  txMsg.can_id = 4;
  txMsg.can_dlc = strlen(text);

  memcpy(txMsg.data, text, txMsg.can_dlc);

  mcp2515.sendMessage(&txMsg);
  Serial.println("SETUP!");
}

void loop() {

  //Code to test received CAN message
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

      carCount++;
      cars.insert({carCount, received});

      //Testing if it's put in the map correctly
      char getS[9];
      strncpy(getS, cars.at(carCount).c_str(), sizeof(getS));
      getS[sizeof(getS) - 1] = '\0';

      Serial.println(getS);
    }
  }


  //Test code for a RFID scanner (still need to implement paying logic)
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