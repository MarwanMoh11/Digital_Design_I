#include <iostream>
#include "LogicGates.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

// Struct to hold information about a logic gate
struct gate {
    LogicGates g;               // LogicGates object representing the gate
    vector<string> ins;         // Vector to store input names
    string out;                 // Output name
};

// Function to read logic gates from a library file
vector<LogicGates> readlib(string x) {
    ifstream inputFile(x);      // Input file stream
    vector<LogicGates> gates;   // Vector to store LogicGates objects
    string array[3];            // Array to temporarily store gate information

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return {};
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
        // Create a LogicGates object and push it into the vector
        gates.push_back(LogicGates(array[0], stoi(array[1]), stoi(array[2])));
    }

    inputFile.close();  // Close the input file
    return gates;       // Return the vector of LogicGates objects
}

// Function to read circuit information from a file
void readcirc(string x, unordered_map<string, pair<bool, int>>& inputs, vector<gate>& gates) {
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
            int i = 0;
            istringstream iss(line);    // String stream to parse the line
            gate x;
            while (getline(iss, word, ',')) {
                switch (i) {
                case 0: continue; break;           // Skip the first word
                case 1: x.g.setName(word); break;  // Set gate name
                case 2: x.out = word; break;       // Set output name
                default: x.ins.push_back(word);    // Add input name to vector
                }
                i++;
            }
            gates.push_back(x);   // Add the gate to the vector of gates
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
            case 1: tempInput = word; break;         // Store input name
            case 2: inputs[tempInput] = { stoi(word), tempDelay }; // Store input logic value and delay
            }
            i++;
        }
    }
}

int main() {
    readlib("C:\\Users\\mazin\\OneDrive\\Desktop\\example.txt"); 
    cout << endl << !true;  

    return 0;
}
