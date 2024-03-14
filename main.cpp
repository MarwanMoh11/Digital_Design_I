#include <iostream>
#include "LogicGates.h"
#include "LogicGates.cpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "GateCompiler.h"
#include "GateCompiler.cpp"
using namespace std;

//Struct to hold information about a logic gate
struct component {
    LogicGates g;               // LogicGates object representing the gate
    vector<string> ins;         // Vector to store input names
    string out;                 // Output name
};

// Function to read logic gates from a library file
void readlib(string x,vector<LogicGates>& y) {
    ifstream inputFile(x);      // Input file stream
    vector<LogicGates> gates;   // Vector to store LogicGates objects
    string array[3];            // Array to temporarily store gate information

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
    }

    string line, word;
    while (getline(inputFile, line)) {
        int i = 0;
        istringstream iss(line);   // String stream to parse the line
        while (getline(iss, word, ',')) {
            cout << word;           // Print the word (for debugging)
            array[i] = word;       // Store the word in the temporary array
            if (i == 2)            // Ignore Logic of Gate
                continue;
            else
                i++;
        }
        cout << endl;
        // Create a LogicGates object and push it into the vector
        gates.push_back(LogicGates(array[0], stoi(array[1]), stoi(array[2])));
    }

    inputFile.close();  // Close the input file
    y = gates;      // equate by reference
}

// Function to read circuit information from a file
void readcirc(string x, unordered_map<string, pair<bool, int>>& inputs, vector<component>& gates) {
    ifstream inputFile(x);    // Input file stream
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line, word;
    while (getline(inputFile, line)) {
        if (line == "INPUTS:") {
            // Read input names until "COMPONENTS:" is encountered
            while (getline(inputFile, line) && line != "COMPONENTS:") {
                inputs[line] = { false,0 };   // Store input name and default logic value
            }
        }
        else {
            if(line != "COMPONENTS:") {
                int i = 0;
                istringstream iss(line);    // String stream to parse the line
                component x; string tempword;
                while (getline(iss, word, ',')) {
                    stringstream ss;
                    ss << word;
                    ss >> tempword;
                    switch (i++) {
                        case 0:
                            continue;
                            break;           // Skip the first word
                        case 1:
                            if(tempword=="NOT" || tempword == "BUFFER"){

                            x.g.setName(tempword);}
                            else {
                                int end = tempword.size();
                                string newword = tempword.substr(0,end-1);
                                x.g.setName(newword);
                            }
                            break;  // Set gate name
                        case 2:
                            x.out = tempword;
                            break;       // Set output name
                        default:
                            x.ins.push_back(tempword);    // Add input name to vector
                    }

                }
                gates.push_back(x);   // Add the gate to the vector of gates
            }
        }
    }
}

// Function to read stimulus information from a file
void readstim(string x, unordered_map<string, pair<bool, int>>& inputs) {
    ifstream inputFile(x);    // Input file stream
    int tempDelay;            // Temporary variable to store delay
    string tempInput;         // Temporary variable to store input name
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line, word;
    while (getline(inputFile, line)) {
        int i = 0;
        istringstream iss(line);    // String stream to parse the line
        while (getline(iss, word, ',')) {
            switch (i) {
            case 0: tempDelay = stoi(word); break;   // Store delay
                case 1: {
                    // Remove leading and trailing whitespace from the word
                    stringstream ss;
                    ss << word;
                    ss >> tempInput;
                    break;
                }
                case 2: inputs[tempInput] = { stoi(word), tempDelay }; // Store input logic value and delay
            }
            i++;
        }
    }
}

int main() {
    vector<LogicGates> gates;
    vector<component> usedgates;
    unordered_map<string, pair<bool, int>> inputs;
    unordered_map<string, bool> inputs1;
    readlib("examplelib.txt", gates);
    readcirc("examplecirc.txt",inputs,usedgates);
    readstim("examplestim.txt",inputs);
    cout << endl;

    // Output each variable in a neat order for debugging
    cout << "Vector of LogicGates:" << endl;
    for (const auto& g : gates) {
        cout << "LogicGates details: " << endl;
        cout << "Name: " << g.getname() << endl;
        cout << "Delay: " << g.getdelay()<< endl;
        cout << "Number of inputs: " << g.getnuminputs() << endl;
        cout << endl;
    }

    cout << endl;

    // Output details of each gate object
    for (const auto& g : usedgates) {
        cout << "Gate Information:" << endl;
        cout << "Output name: " << g.out << endl;
        cout << "Input names: ";

        cout << endl;
        // Optionally, you can output details of the LogicGates object if needed
        cout << "LogicGates details: " << endl;
        cout << "Name: " << g.g.getname() << endl;
        cout << "Delay: " << g.g.getdelay()<< endl;
        cout << "Number of inputs: " << g.g.getnuminputs() << endl;
        cout << endl;
    }

    cout << "Inputs:" << endl;
    for (const auto& input : inputs) {
        cout << "Input Name: " << input.first << ", Logic Value: " << input.second.first << ", Delay: " << input.second.second << endl;
    }

    vector<string> x = infixToPostfix("(i&~j)|(~i&j)");
    cout<<evaluatePostfix(x, inputs1, usedgates.back().ins);




    return 0;
}
