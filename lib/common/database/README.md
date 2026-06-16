# Parkify Database Library

A synchronous, easy-to-use C++ library for interacting with the Central Database Node over CAN bus.

## Features

- **Synchronous API:** Methods block until a response is received or a timeout occurs.
- **Optimized Communication:** All session data is packed into a single 8-byte CAN frame.
- **Error Handling:** Uses `std::optional` and `SpecialID` constants for robust error detection.
- **DRY Design:** Internally handles message IDs, DLCs, and data marshaling.

## Installation

Ensure your node's `platformio.ini` includes:
- `autowp/autowp-mcp2515` (CAN controller library)
- Access to the `lib/common` folder.

## Getting Started

### 1. Setup

Instantiate the `DatabaseClient` by passing a reference to your initialized `MCP2515` object.

```cpp
#include <mcp2515.h>
#include "database/database_client.h"

MCP2515 mcp(CAN_CS_PIN);
DatabaseClient dbClient(mcp);

void setup() {
    // Standard SPI and MCP2515 initialization
    SPI.begin();
    mcp.reset();
    mcp.setBitrate(CAN_500KBPS, MCP_8MHZ);
    mcp.setNormalMode();
}
```

## core Functions

### 2. Requesting a New ID (Entry Gate)
Used to obtain a unique ID for a vehicle entering the garage.
```cpp
uint16_t parkingId = dbClient.requestLowestId();

if (parkingId != SpecialID::NOT_FOUND) {
    Serial.printf("Assigned unique ID: %u\n", parkingId);
}
```

### 3. Fetching Full Session Data
Retrieve status, entry time, and plate info in one call.
```cpp
auto session = dbClient.requestSession(123);

if (session.has_value()) {
    Serial.printf("Plate: %s | Paid: %s | Time: %u\n", 
                  session->licenceplate.data(), 
                  session->payed ? "Yes" : "No",
                  session->created_at);
}
```

### 4. Search ID by Plate (RFID)
Find a Parking ID using a 3-character plate/RFID string.
```cpp
uint16_t id = dbClient.requestIdByPlate("524");

if (id != SpecialID::NOT_FOUND) {
    // Found vehicle ID 123 for tag 524
}
```

### 5. Verify Payment Status
Simplified helper to check if a vehicle is cleared to leave.
```cpp
if (dbClient.isPaid(123)) {
    Serial.println("Payment verified. Opening gate...");
}
```

### 6. Marking Session as Paid
Updates the database and **waits for a confirmation broadcast**.
```cpp
if (dbClient.setSessionPaid(123)) {
    Serial.println("Database successfully updated status to PAID");
} else {
    Serial.println("Update failed or timed out");
}
```

## Data Protocols

### Vehicle Registration (Entry Flow)
To ensure data integrity, use this two-step broadcast pattern:
1. **Notice:** Broadcast `BR_ENTRY_NOTICE` (ID 3) containing the `uint16_t` ID.
2. **Registration:** Send `TX_ENTRY_REGISTRATION` (ID 4) containing the 3-character plate string.

### Single Frame Optimization
The library uses an optimized 8-byte payload for `TX_SESSION_DATA` (ID 11):
- **Byte 0:** Payment Status (0: Unpaid, 1: Paid)
- **Byte 1-4:** Created At (Unix Timestamp)
- **Byte 5-7:** Licence Plate (3 characters)

## Advanced Usage (FreeRTOS)
While the API is synchronous, it is **thread-safe** and **RTOS-friendly**. If you need to perform other tasks while waiting for the database, simply wrap the client calls in a dedicated FreeRTOS Task. This prevents the blocking call from freezing your main application logic.
