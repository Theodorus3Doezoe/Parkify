#include "io/ConsoleInputReader.h"

String ConsoleInputReader::readInput() {
    if (Serial.available()) {
        return Serial.readStringUntil('\n');
    }
    return "";
}
