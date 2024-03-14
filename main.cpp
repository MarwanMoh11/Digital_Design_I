#include <iostream>
#include "LogicGates.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<unordered_map>
using namespace std;

struct gate {
    LogicGates g;
    vector<string> ins;
    string out;
};


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

void readcirc(string x, unordered_map <string, pair<bool, int>> &inputs, vector<gate> &gates)
{
    ifstream inputFile(x);
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line, word;
    while (getline(inputFile, line))
    {
        if (line == "INPUTS:")
        {
            while (getline(inputFile, line) && line != "COMPONENTS:")
            {
                inputs[line] = { false,0 };
            }

        }
        else
        {

                int i = 0;
                istringstream iss(line);
                gate x;
                while (getline(iss, word, ','))
                {
                    switch (i)
                    {
                    case 0: continue; break;
                    case 1: x.g.setName(word); break;
                    case 2: x.out = word; break;
                    default: x.ins.push_back(word);
                    }
                    i++;
                }
                gates.push_back(x);
            }
  


    }
}
 
void readstim(string x, unordered_map <string, pair<bool, int>>& inputs) {
    ifstream inputFile(x); int tempDelay; string tempInput;
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
        string line, word;
        while (getline(inputFile, line))
        {
            int i = 0;
            istringstream iss(line);
            while (getline(iss, word, ','))
            {
                switch (i)
                {
                case 0: tempDelay = stoi(word); break;
                case 1: tempInput = word; break;
                case 2: inputs[tempInput] = { stoi(word), tempDelay };
             }
                i++;
            }
        }


    }
}

int main()
{
    readlib("C:\\Users\\mazin\\OneDrive\\Desktop\\example.txt");
    cout << endl << !true;
 
    return 0;
}
