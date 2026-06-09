#include "io/ConsoleInputReader.h"

String ConsoleInputReader::readInput() {
    if (Serial.available()) {
        char c = (char)Serial.read();
        return String(c);
    }
    return "";
}
