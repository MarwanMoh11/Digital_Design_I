#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <cctype>
#include <queue>
#include "Gates.h"
#include "Outputs.h"
#include "component.h"
#include "Postfix_Functions.h"
#include "readInput_Functions.h"
#include "Input.h"
using namespace std;

int main() {
    vector<Gates> gates;
    unordered_map<string, Input> map;
    vector<component> c;
    priority_queue<Input> inputs;
    unordered_map<string, pair<bool, int>> curr;

    // Get file names from the user
    string libFile, circFile, stimFile;
    cout << "Enter lib file path (0 for default):";
    cin >> libFile;
    libFile = (libFile == "0") ? "examplelib.txt" : libFile;

    cout << "Enter circ file path (0 for default):";
    cin >> circFile;
    circFile = (circFile == "0") ? "examplecirc.txt" : circFile;

    cout << "Enter stim file path (0 for default):";
    cin >> stimFile;
    stimFile = (stimFile == "0") ? "examplestim.txt" : stimFile;
    cout << endl;

    cout << "Reading library file: " << libFile << endl;
    readlib(libFile, gates);
    cout << "Library file read successfully." << endl;

    cout << "Reading circuit file: " << circFile << endl;
    readcirc(circFile, inputs, c);
    cout << "Circuit file read successfully." << endl;

    int noIns = inputs.size();
    cout << "Number of inputs: " << noIns << endl;

    priority_queue<Input> temp = inputs;
    while (!temp.empty()) {
        curr[temp.top().name] = { 0, 0 };
        temp.pop();
    }
    cout << "Current map initialized." << endl;

    cout << "Reading stimulus file: " << stimFile << endl;
    readstim(stimFile, inputs);
    cout << "Stimulus file read successfully." << endl;

    vector<bool> tempins;
    string logic;
    int maxx = 0;
    int tempDelay = 0;

    for (int i = 0; i < c.size(); i++) {
        temp = inputs;
        for (int z = 0; z < c[i].ins.size(); z++) {
            tempins.push_back(curr[c[i].ins[z]].first);
        }
        for (int z = 0; z < tempins.size(); z++) {
            cout << tempins[z] << endl;
        }
        for (int z = 0; z < gates.size(); z++) {
            if (c[i].name == gates[z].name) {
                logic = gates[z].logic;
                tempDelay = gates[z].delay;

            }
        }

        for (int j = 0; j < inputs.size(); j++) {
            maxx = 0;
            for (int z = 0; z < c[i].ins.size(); z++) {
                if(!temp.empty())
                if (c[i].ins[z] == temp.top().name) {
                    curr[temp.top().name] = { temp.top().value, temp.top().time_stamp };
                    tempins[z] = temp.top().value;
                    temp.pop();
                    maxx = max(maxx, curr[c[i].ins[z]].second);
                    inputs.push(Input(c[i].out, evaluatePostfix(infixToPostfix(logic), tempins), maxx + tempDelay));
                }
            }
        }
        tempins.clear();
    }


    cout << "Simulation results:" << endl;
    while (!inputs.empty()) {
        Input input = inputs.top();
        inputs.pop();
        cout << "Name: " << input.name << ", Value: " << input.value << ", Delay: " << input.time_stamp << endl;
    }

    return 0;
}
