#include "Payment.h"

Payment::Payment(){}

int maxTries = 5;

bool Payment::ChoosePayment(){
    cout << "How do you want to pay?" << endl;
    cout << "1. By Cash." << endl;
    cout << "2. By card." << endl;

    int paymentChoise;

    bool payLoop = true;

    while (payLoop)
    {
        try
        {
            cin >> paymentChoise;

            if(paymentChoise == 1)
            {
                cout << "You just payed by cash.\nHave a good day.\n";
                return true;
            }
            else if (paymentChoise == 2)
            {
                int pin;
                int currentTries;
                cout << "Enter a valid pin:";
                bool pinLoop = true;
                while (pinLoop)
                {
                    try
                    {
                        if (currentTries == maxTries)
                        {
                            cout << "You failed to enter your pin correctly.\n";
                            return false;
                        }
                        
                        cin >> pin;
                        if(pin > 999 && pin < 10000)
                        {
                            cout << "Pin is valid.\n";
                            pinLoop = false;
                            return true;
                        }
                        else
                        {
                            currentTries++;
                        }
                    }
                    catch(const std::exception& e)
                    {
                        cout << "Enter a valid pin using only numbers.\n";
                        currentTries++;
                    }
                    
                    
                }
                
            }
            
            else
            {
                cout << "You can only choose 1 or 2" << endl;
            }
        }
        catch(const std::exception& e)
        {
            cout << "You can only put in a number." << endl;
        }
    }
    return false;
}