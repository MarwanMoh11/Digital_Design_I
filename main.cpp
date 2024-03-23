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

    for (auto it = map.begin(); it != map.end(); ++it) {
        outputs temp(it->second.second,it->first,it->second.first);
        pq.push(temp);
    }

    for (int i = 0; i < c.size(); i++)
    {

        for (int j = 0; j < y.size(); j++)
        {
            if (c[i].name == y[j].name)
            {
                evaluatePostfix(infixToPostfix(y[j].logic), map, c, i,y[j].delay);
                outputs temp(map[c[i].out].second,c[i].out,map[c[i].out].first);
                pq.push(temp);
               break;
            }
        }
    }


    ofstream outputFile("simulation.sim");
    if (outputFile.is_open()) {
        // Temporarily redirect cout to outputFile
        streambuf* coutBuffer = cout.rdbuf();
        cout.rdbuf(outputFile.rdbuf());

        // Output to cout (which is redirected to outputFile)
        while(!pq.empty()){
            cout << pq.top().time_stamp_ps << ", " << pq.top().logic_value << ", " << pq.top().input << endl;
            pq.pop();
        }

        // Restore cout
        cout.rdbuf(coutBuffer);

        // Close the file
        outputFile.close();
        cout << "Output written to simulation.sim" << endl;
    } else {
        cout << "Error: Unable to open file simulation.sim" << endl;
    }







    return 0;
}




