#ifndef component_H
#define component_H
#include <iostream>
#include <vector>
using namespace std;


//Struct to hold information about a logic gate
struct component {
    string name;               // LogicGates object representing the gate
    vector<string> ins;         // Vector to store input names
    string out;                 // Output name


};

#endif 
