#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
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
    priority_queue<Input> temp;
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
    readcirc(circFile, inputs, c, curr, gates);
    cout << "Circuit file read successfully." << endl;


    cout << "Current map initialized." << endl;

    cout << "Reading stimulus file: " << stimFile << endl;
    readstim(stimFile, inputs, curr);
    cout << "Stimulus file read successfully." << endl;


    for (auto& pair : curr) {
        inputs.push(Input(pair.first, pair.second.first, pair.second.second));
    }



    vector<bool> tempins;
    string logic;
    int maxx = 0;
    int tempDelay = 0;

    for (int i = 0; i < c.size(); i++) {
        temp = inputs;
        for (int z = 0; z < c[i].ins.size(); z++) {
            tempins.push_back(curr[c[i].ins[z]].first);
        }
        for (int z = 0; z < gates.size(); z++) {
            if (c[i].name == gates[z].name) {
                logic = gates[z].logic;
                tempDelay = gates[z].delay;
            }
        }

        int fixedsize = temp.size(); // Use the size of temp instead of inputs
        for (int j = 0; j < fixedsize; j++) {
            maxx = 0;
            bool found = false;
            if (!temp.empty()) {
                for (int z = 0; z < c[i].ins.size(); z++) {
                    if (c[i].ins[z] == temp.top().name) {
                        found = true;
                        curr[temp.top().name] = { temp.top().value, temp.top().time_stamp };
                        tempins[z] = temp.top().value;
                        maxx = max(maxx, curr[c[i].ins[z]].second);
                        inputs.push(Input(c[i].out, evaluatePostfix(infixToPostfix(logic), tempins), maxx + tempDelay));

                    }
                }
                temp.pop(); // Always pop the top element from temp
            }
        }

        tempins.clear();
    }

    unordered_map<string, bool> lastValueMap;
    for (auto& pair : curr) {
        lastValueMap[pair.first] = pair.second.first;
    }


    unordered_set<string> seenOutputs;
    bool written = false; // To check if output is written in sim file
    ofstream outputFile("simulation.sim"); // Open the file for writing

    if (!outputFile.is_open()) {
        cout << "Error: Unable to open output file." << std::endl;
        return 1; // Return with an error code if file opening fails
    }


    while (!inputs.empty()) {
        Input input = inputs.top();
        inputs.pop();

        // Check if the value has changed from the last time
        if (lastValueMap[input.name] != input.value) {
            // Update the last value map
            lastValueMap[input.name] = input.value;

            // Create a string representation of the output
            stringstream ss;
            ss << input.time_stamp << ", " << input.name << ", " << input.value;
            string outputStr = ss.str();

            // Check if the output has already been printed
            if (seenOutputs.find(outputStr) == seenOutputs.end()) {
                // If it hasn't been printed, add it to the set and write it to the file
                seenOutputs.insert(outputStr);
                outputFile << outputStr << endl;
            }
        }
        written = true;
    }
    if(written) cout << "Output written to simulation.sim";

    outputFile.close(); // Close the file
    return 0;
}