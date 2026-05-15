#pragma once

#include "interfaces/IInputReader.h"

class ConsoleInputReader : public IInputReader {
public:
    String readInput() override;
};
