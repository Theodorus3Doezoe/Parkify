#ifndef SCREEN_H
#define SCREEN_H

#include "IScreen.h"
#include <string>
#include <iostream>
#include "json.hpp"

class Screen : public IScreen
{
private:
    
public:
    Screen();

    int Menu(int amount, string fileName) override;
};


#endif