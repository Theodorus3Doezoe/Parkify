#ifndef ISCREEN_H
#define ISCREEN_H
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class IScreen{
    public:
    virtual int Menu(int amount, string fileName) = 0;
};
#endif