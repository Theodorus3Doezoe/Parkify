#include "Screen.h"
using json = nlohmann::json;

Screen::Screen(){}

int Screen::Menu(int amount, string fileName){
    json jsonData;
    ifstream inputFile(fileName);

    inputFile >> jsonData;

    inputFile.close();

    cout << "Choose which car:\n";
    for (int i = 1; i <= amount; i++)
    {
        string jsonEntry = "car";
        jsonEntry += to_string(i);
        cout << i << ". " << jsonData[jsonEntry]["licensePlate"] << endl;
    }
    int carChoise;
    bool carLoop = true;
    while (carLoop)
    {
        try
        {
            cin >> carChoise;

            if(carChoise <= amount)
            {
                return carChoise;
            }
            else
            {
                cout << "You can only choose from 1 to " << amount << endl;
            }
        }
        catch(const std::exception& e)
        {
            cout << "You can only put in a number." << endl;
        }
    }
    return 0;
}