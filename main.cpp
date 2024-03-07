#include <iostream>
#include "LogicGates.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<unordered_map>
using namespace std;

vector<LogicGates> readlib(string x)
{
    ifstream inputFile(x); vector<LogicGates> gates; string array[3];
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return {};
    }

    string line, word;
    while (getline(inputFile, line))
    {
        int i = 0;
        istringstream iss(line);
        while (getline(iss, word, ','))
        {
            cout << word;
            array[i] = word;
            if (i == 2) //Ignore Logic of Gate
                continue;
            else i++;
        }
        gates.push_back(LogicGates(array[0], stoi(array[1]), stoi(array[2])));
    }

    inputFile.close(); 
    return gates;
}

void readcirc(string x)
{
    unordered_map<string, bool> inputs;
    ifstream inputFile(x);  
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return ;
    }

    string line, word;
    while (getline(inputFile, line))
    {
        if (line == "INPUTS:")
        {
            while (getline(inputFile, line) && line != "COMPONENTS:")
            {
                inputs[line] = false;
            }

        }
        

}
    ]

int main()
{
    readlib("C:\\Users\\mazin\\OneDrive\\Desktop\\example.txt");
    cout << endl << !true;
 
    return 0;
}
