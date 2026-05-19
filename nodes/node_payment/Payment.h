#ifndef PAYMENT_H
#define PAYMENT_H

#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class Payment{
    public:
    Payment();
    bool ChoosePayment();
};
#endif