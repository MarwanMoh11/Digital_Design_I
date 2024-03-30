#ifndef readInput_Funtions_H
#define readInput_Funtions_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Gates.h"
#include "component.h"
#include "Input.h"
using namespace std;

// Function to read logic gates from a library file
void readlib(string x, vector<Gates>& y) {
    ifstream inputFile(x);      // Input file stream
    vector<Gates> gates;   // Vector to store LogicGates objects
    string array[4];            // Array to temporarily store gate information

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
    }

    string line, word;
    while (getline(inputFile, line)) {
        int i = 0;
        istringstream iss(line);   // String stream to parse the line
        while (getline(iss, word, ',')) {
            // Print the word (for debugging)
            array[i] = word;       // Store the word in the temporary array
            i++;
        }

        Gates g;
        g.name = array[0];
        g.numInputs = stoi(array[1]);
        g.logic = array[2];
        g.delay = stoi(array[3]);

        gates.emplace_back(g);
    }

    inputFile.close();  // Close the input file
    y = gates;      // equate by reference
}

// Function to read circuit information from a file
void readcirc(string x, priority_queue<Input> & inputs, vector<component>& gates,unordered_map<string, pair<bool, int>>& curr, vector<Gates> y) {
    ifstream inputFile(x);    // Input file stream
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    bool valid;
    string line, word; 
    while (getline(inputFile, line)) {
        valid = false;
        if (line == "INPUTS:") {
            // Read input names until "COMPONENTS:" is encountered
            while (getline(inputFile, line) && line != "COMPONENTS:") {
                curr[line] = {0,0};
            }
        }
        else {
            if (line != "COMPONENTS:") {
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
                        break;
                    case 1:
                        for (int j = 0; j < y.size(); j++)
                            if (y[j].name == tempword)
                                valid = true;
                        if (!valid)
                        {
                            cout << "Gate "<< tempword<<" Not Found in Library: Program will be terminated";
                            exit(EXIT_SUCCESS);
                        }
                        x.name = tempword;
                        break;  // Set gate name
                    case 2:
                        if (curr.find(tempword) != curr.end())
                        {
                            cout << "Invalid Output, Clashing wires in " << x.name << " Gate: Program will be terminated";
                            exit(EXIT_SUCCESS);
                        }
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
void readstim(string x, priority_queue<Input>& inputs,unordered_map<string, pair<bool, int>>& curr) {
    ifstream inputFile(x);    // Input file stream
    int tempDelay;            // Temporary variable to store delay
    string tempInput;         // Temporary variable to store input name
    bool valid=true;
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line, word;
    while (getline(inputFile, line)) {
        int i = 0;
        valid = true;
        istringstream iss(line);    // String stream to parse the line
        while (getline(iss, word, ',')) {

            switch (i) {
            case 0: tempDelay = stoi(word); break;   // Store delay
            case 1: {
                // Remove leading and trailing whitespace from the word
                stringstream ss;
                ss << word;
                ss >> tempInput;
                if (curr.find(tempInput) == curr.end())
                {
                    cout << "Invalid Input in stim file: Input " << tempInput << " will be ignored" << endl;
                    valid = false;
                }

                break;
            }
            case 2: {
                if (valid)
                {
                    if (stoi(word) != 0 && stoi(word) != 1)
                    {
                        cout << "Input value for " << tempInput << " at time stamp "<< tempDelay<<" is invalid: Value will be set to 0" << endl;
                        word = "0";
                  }
                    if (tempDelay == 0 && stoi(word) == 1) {
                      curr[tempInput] = { 1, 0 };
                  }
                  else if (tempDelay != 0) {
                      inputs.push(Input(tempInput, stoi(word), tempDelay)); // Store input logic value and delay}
                  }
            }
        }
            
            }
            i++;
            
        }
    }
}







#endif // !readInput_Funtions_H