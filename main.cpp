#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include<stack>
#include<cctype>
#include <queue>
#include "Gates.h"
#include "component.h"
#include "Outputs.h"
#include "Postfix_Functions.h"
#include "readInput_Functions.h"
using namespace std;



int main() {
    priority_queue<outputs> pq;
    stack<bool> operands;
    vector<Gates> y; unordered_map<string, pair<bool, int>> map; vector<component> c;
    readlib("examplelib.txt", y);
    readcirc("examplecirc.txt", map, c);
    readstim("examplestim.txt", map);
    unordered_map<string,bool> inputs;
    unordered_map<string,bool> previousValues;

    // Initialize the inputs and store them with a constructor
    for (auto it = map.begin(); it != map.end(); ++it) {
        outputs temp(it->second.second,it->second.first,it->first);
        pq.push(temp);
        previousValues[it->first] = 0; // Initialize previous values to 0
    }

    for (int i = 0; i < c.size(); i++)
    {
        for (int j = 0; j < y.size(); j++)
        {
            if (c[i].name == y[j].name) //if the component name is the same as the gate name
            {
                evaluatePostfix(infixToPostfix(y[j].logic), map, c, i,y[j].delay, pq);//evaluate the postfix expression
                outputs temp(map[c[i].out].second,map[c[i].out].first,c[i].out);//store the output in a temporary variable
                pq.push(temp);//push the output to the priority queue
                break;
            }
        }
    }

    ofstream outputFile("simulation.sim");//open the output file
    if (outputFile.is_open()) {
        // Temporarily redirect cout to outputFile
        streambuf* coutBuffer = cout.rdbuf();
        cout.rdbuf(outputFile.rdbuf());

        // Output to cout (which is redirected to outputFile)
        while(!pq.empty()){
            if (pq.top().logic_value == 1 && previousValues[pq.top().input] == 0) {
                cout << pq.top().time_stamp_ps << ", " << pq.top().logic_value << ", " << pq.top().input << endl;
            }
            previousValues[pq.top().input] = pq.top().logic_value;
            pq.pop();
        }

        // Restore cout to the terminal
        cout.rdbuf(coutBuffer);

        // Close the file
        outputFile.close();
        cout << "Output written to simulation.sim" << endl; // Output to console
    } else {
        cout << "Error: Unable to open file simulation.sim" << endl; // Output to console
    }

    return 0;
}



