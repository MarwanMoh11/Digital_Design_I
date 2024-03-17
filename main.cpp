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
using namespace std;
struct outputs{

    outputs(int x, string y, bool z){
        time_stamp_ps = x;
        input = y;
        logic_value = z;
    }

    bool operator<(const outputs& other) const {
        // This defines the order in the priority queue based on priority field
        return time_stamp_ps > other.time_stamp_ps;
    }

    int time_stamp_ps;
    string input;
    bool logic_value;
};

bool isin(const string& str) {
    bool hasAlphabetic = false;
    bool hasNumeric = false;

    for (char ch : str) {
        if (std::isalpha(ch)) {
            hasAlphabetic = true;
        } else if (std::isdigit(ch)) {
            hasNumeric = true;
        }

        // If both alphabetic or numeric characters are found, return true
        if (hasAlphabetic || hasNumeric) {
            return true;
        }
    }

    // If neither alphabetic nor numeric characters are found, return false
    return false;
}


//support function to calculate precedence of logic operators

int precedence(char op) { 
    if (op == '~')
        return 3;
    if (op == '&')
        return 2;
    if (op == '|')
        return 1;
    return 0;
}
string charToString(char c) {
    return string(1, c); // Converts a character into a string of size 1
}
vector<string> infixToPostfix(const string& infix) {
    stack<char> operators;
    vector<string> postfix;
    string temp;

    for (char ch : infix) {
        if (isalpha(ch)) {
            temp.push_back(ch);
        }
        else if (isdigit(ch)) {
            temp.push_back(ch);
        }
        else {
            if (!temp.empty()) {
                postfix.push_back(temp);
                temp.clear();
            }
            if (ch == '(') {
                operators.push(ch);
            }
            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix.push_back(charToString(operators.top()));
                    operators.pop();
                }
                operators.pop(); // Discard '('
            }
            else { // Operator
                while (!operators.empty() && precedence(ch) <= precedence(operators.top())) {
                    postfix.push_back(charToString(operators.top()));
                    operators.pop();
                }
                operators.push(ch);
            }
        }
    }

    if (!temp.empty()) {
        postfix.push_back(temp);
        temp.clear();
    }

    while (!operators.empty()) {
        postfix.push_back(charToString(operators.top()));
        operators.pop();
    }

    return postfix;
}





bool evaluatePostfix(const vector<string>& postfix, unordered_map<string, pair<bool, int>>& map,  vector<component>& gs, int i, int delay) {
    stack<bool> operands;
    int biggestinput = 0;
    for (const string& token : postfix) {
        if (isalpha(token[0])) {
            // If token is an input variable, fetch its value from the inputs vector
            int index = token[1] - '1'; // Assuming inputs are named as i1, i2, ..., and their values are provided in inputs vector
            operands.push(map[gs[i].ins[index]].first);
            biggestinput = max(biggestinput, map[gs[i].ins[index]].second);


        }else {

            if (token == "~") {
                // Negation operator
                if (!operands.empty()) {
                    bool operand = operands.top();
                    operands.pop();
                    operands.push(!operand);

                } else {
                    // Handle error: Missing operand
                    // This could be due to incorrect postfix expression
                    cerr << "Error: Missing operand for negation operator '~'" << endl;
                    return false;
                }
            }
            else {

                // Binary operators: '&' and '|'
                if (operands.size() < 2) {
                    // Handle error: Missing operands
                    // This could be due to incorrect postfix expression
                    map[gs[i].out].first = operands.top();
                    map[gs[i].out].second = map[gs[i].out].second + delay + biggestinput;
                    return false;
                }
                bool operand2 = operands.top();
                operands.pop();
                bool operand1 = operands.top();
                operands.pop();
                if (token == "&") {
                    operands.push(operand1 && operand2);

                }
                else if (token == "|") {
                    operands.push(operand1 || operand2);


                }
            }
        }


    }

    if (operands.size() != 1) {
        // Handle error: Invalid expression
        // This could be due to incorrect postfix expression
        cerr << "Error: Invalid expression" << endl;
        return false;
    }



    return operands.top(); // The final result after evaluating the entire postfix expression
}




// Function to read logic gates from a library file
void readlib(string x,vector<Gates>& y) {
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
                            break;         
                        case 1:
                            x.name = tempword;
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


    ofstream outputFile("simulation.txt");
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
        cout << "Output written to simulation.txt" << endl;
    } else {
        cerr << "Error: Unable to open file simulation.txt" << endl;
    }







    return 0;
}




