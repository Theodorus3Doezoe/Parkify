#ifndef KEYPAD_H
#define KEYPAD_H
#include "IKeypad.h"

#include <iostream>

using namespace std;

class Keypad : public IKeypad{
    public:
    Keypad();
    bool EnteringPin() override;
};

#endif