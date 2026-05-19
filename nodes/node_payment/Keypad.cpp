#include "Keypad.h"

Keypad::Keypad(){}

bool Keypad::EnteringPin()
{
    cout << "Enter pin: " ;

    int pin;
    bool loop = true;
    int chances = 5;
    int currentTries = 0;
    while (loop)
    {
        try
        {
            if (chances == currentTries)
            {
                cout << "You only had 5 tries" << endl;
                return false;
            }
            
            cin >> pin;

            if(pin <= 10000 && pin >= 999)
            {
                loop = false;
            }
            else
            {
                cout << "Enter a valid pin (1000 -> 9999)" << endl;
                currentTries++;
            }
        }
        catch(const std::exception& e)
        {
            cout << "You can only use numbers" << endl;
            currentTries++;
        }
        
    }
    
    return true;

}