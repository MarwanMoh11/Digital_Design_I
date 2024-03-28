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
    priority_queue<outputs> pq;


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

    readlib(libFile, gates);
    readcirc(circFile, map, c);
    readstim(stimFile, map);
    
    vector<bool> inputs;
    vector<int> delays;
    int tempMax=0;
    int counter = 0;
    int maxUsed = 0;
    int tempMin = 1000000000;
    string tempName;
    for (int i = 0; i < c.size(); i++)
    {
        for (int j = 0; j < c[i].ins.size(); j++)
        {
            maxUsed += map[c[i].ins[j]].used - 1;
       }
        
        
        while(counter < maxUsed)
        {
            for (int j = 0; j < c[i].ins.size(); j++)
            {
                inputs.push_back(map[c[i].ins[j]].values[map[c[i].ins[j]].used].first);
                delays.push_back(map[c[i].ins[j]].values[map[c[i].ins[j]].used].second);
                tempMax = max(tempMax, delays.back());
                counter += map[c[i].ins[j]].used;
                if (map[c[i].ins[j]].values[map[c[i].ins[j]].used + 1].second < tempMin)
                {
                    tempMin = map[c[i].ins[j]].values[map[c[i].ins[j]].used + 1].second;
                    tempName = map[c[i].ins[j]].name;
                }

            }
            for (int z = 0; z < gates.size(); z++)
            {
                if (gates[z].name == c[i].name)
                {
                    
                    if (map.find(c[i].out) == map.end())
                        map[c[i].out] = Input(c[i].out, evaluatePostfix(infixToPostfix(gates[z].logic), inputs), tempMax + gates[z].delay);
                    else
                        map[c[i].out].values.push_back({ evaluatePostfix(infixToPostfix(gates[z].logic), inputs) , tempMax + gates[z].delay });
                    map[tempName].used++;
                    tempMax = 0;
                    delays.clear();
                    inputs.clear();
                    counter = 0;
                }
                break;

            }

        }
        maxUsed = 0;

    }
    
    for (auto entry : map) {
        // Loop over the values vector in Input
        for (auto value : entry.second.values) {
            // Create an outputs object and push it into the priority queue
            outputs output(value.second, value.first, entry.first);
            pq.push(output);
            // Print the output values
            cout << "Added output: time_stamp_ps = " << output.time_stamp_ps
                << ", input = " << output.input
                << ", logic_value = " << output.logic_value << endl;
        }
    }



    return 0;
}


