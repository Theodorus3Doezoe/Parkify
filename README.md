# Parking Garage Project

Welcome to the project repository. This project is designed to manage multiple ESP32 nodes (UI, Sensors, Motors, etc.) in a single codebase using PlatformIO.

## Directory Structure

- `nodes/`: Contains all the individual sub-projects (nodes).
  - `node_ui/`: The user interface node.
  - `node_firesys/`: The fire detection system node.
- `lib/common/`: Shared code and configurations.
  - `can_comms_protocol.h`: CAN message ID definitions.
  - `can_config.h`: Shared CAN settings (bitrate, clock) and global `SystemState`.

## How to Work with Nodes

Since we have multiple nodes, you must always specify the environment using the `-e` flag.

### 1. Compiling a Node
To check if your code compiles:
```bash
# For UI Node
pio run -e node_ui

# For Fire System Node
pio run -e node_firesys
```

### 2. Uploading to ESP32
To flash the code to your hardware:
```bash
pio run -e node_ui -t upload
```

### 3. Serial Monitor
To see the debug output and interact with the node:
```bash
# Monitor only
pio device monitor -e node_ui

# Upload AND Monitor (Recommended)
pio run -e node_ui -t upload -t monitor
```

## Adding a New Node

1. Create a new directory: `nodes/node_name/src/` and `nodes/node_name/include/`.
2. Add your code to the `src/` folder.
3. Open `platformio.ini` and add a new environment:

```ini
[env:node_name]
platform = espressif32
board = esp32dev
framework = arduino
build_src_filter = -<*> +<node_name/src/>
build_flags = 
    -I nodes/node_name/include 
    -I lib/common
lib_deps = https://github.com/autowp/arduino-mcp2515.git
```

## Common Library (`lib/common/`)

The `lib/common/` directory contains shared header files that ensure all nodes use the same protocol and configurations.

### Contents:
1.  **`can_comms_protocol.h`**:
    *   Defines the `CAN_MessageID` enum with all project IDs (1-10).
    *   Example: `REQ_OCCUPANCY`, `TX_SPOT_INFO`, `BR_STATE`, etc.
2.  **`can_config.h`**:
    *   **Pins**: `CAN_SPI_CS_PIN` (Default is 5).
    *   **Bus Settings**: `CAN_BITRATE` (500KBPS) and `CAN_CLOCK_SPEED` (8MHZ).
    *   **System State**: The `SystemState` enum (`INIT`, `CLOSED`, `OPEN`, `EMERGENCY`).

### How to use in your Node:

To use these in any node, simply include them at the top of your code. Since `lib/common` is added to the `build_flags` in `platformio.ini`, you don't need to specify the full path:

```cpp
#include "can_comms_protocol.h"
#include "can_config.h"

void setup() {
    // Accessing shared config
    mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK_SPEED);

    // Using shared states
    SystemState currentState = SystemState::INIT;
}

void sendMessage() {
    struct can_frame frame;
    // Using shared protocol IDs
    frame.can_id = BR_STATE; 
    frame.data[0] = SystemState::OPEN;
}
```
