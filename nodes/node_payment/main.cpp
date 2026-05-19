#include <iostream>
#include "json.hpp"
#include "Payment.h"
#include "Screen.h"
#include "Keypad.h"
#include <ctime>
#include <string>
#include <math.h>

using json = nlohmann::json;
using namespace std;

#define FILENAME "cars.json"
#define MAXLICENSPLATE 6

void SetupJSON(int amount);
void changeStatus(int carJSON);

string licensplate[] = {"G412RL", "K883TV", "1SXB42", "6ZND19", "T056BK", "12DGE4"};
json jsonData;

int main()
{
    Payment payment;
    IKeypad* keypad = new Keypad();
    IScreen* screen = new Screen();

    cout << "Before we continue.\nHow many cars should be generated?\n";
    int amount = 0;
    bool loop = true;
    while (loop)
    {
        try
        {
            cin >> amount;
            loop = false;
        }
        catch(const std::exception& e)
        {
            cout << "Only put a number.\n";
        }
        
        

    }
    

    SetupJSON(amount);

    bool hasPaid = false;

    int carChoise = screen->Menu(amount, FILENAME);

    hasPaid = payment.ChoosePayment();
    

    if (hasPaid)
    {
        //change payment status from car
        changeStatus(carChoise);
    }
    else
    {
        cout << "You failed to pay\nSo this programm will now close.\n";
    }
    

    delete keypad;
    delete screen;
    return 0;
}



void SetupJSON(int amount)
{
    //Setup the JSON
    ifstream inputfile(FILENAME);


    for (int i = 1; i <= amount; i++)
    {
        string jsonEntry = "car";
        jsonEntry += to_string(i);
        time_t timestamp;
        time(&timestamp);
        jsonData[jsonEntry]["dateTime"] = ctime(&timestamp);
        if (i > MAXLICENSPLATE)
        {
            int multi = floor(i / MAXLICENSPLATE);
            jsonData[jsonEntry]["licensePlate"] = licensplate[i - (MAXLICENSPLATE * multi)];
        }
        else
        {
            jsonData[jsonEntry]["licensePlate"] = licensplate[i];
        }
        jsonData[jsonEntry]["hasPaid"] = false;
    }
    


    ofstream outputfile(FILENAME);

    outputfile << jsonData.dump(4);

    outputfile.close();

    cout << "JSON file created/loaded\n";

}

void changeStatus(int carJSON)
{
    ifstream inputfile(FILENAME);
    ofstream outputFile(FILENAME);

    inputfile >> jsonData;

    string jsonEntry = "car";
    jsonEntry += to_string(carJSON);
    jsonData[jsonEntry]["hasPaid"] = true;

    outputFile << jsonData.dump(4);

    outputFile.close();
}