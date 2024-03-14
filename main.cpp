#include <iostream>
#include "LogicGates.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include<stack>
#include<cctype>
using namespace std;
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





bool evaluatePostfix(const vector<string>& postfix, const vector<bool>& inputs) {
    stack<bool> operands;

    for (const string& token : postfix) {
        if (isalpha(token[0])) {
            // If token is an input variable, fetch its value from the inputs vector
            int index = token[1] - '1'; // Assuming inputs are named as i1, i2, ..., and their values are provided in inputs vector
            operands.push(inputs[index]);
        }
        else if (token == "~") {
            // Negation operator
            bool operand = operands.top();
            operands.pop();
            operands.push(!operand);
        }
        else {
            // Binary operators: '&' and '|'
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

    return operands.top(); // The final result after evaluating the entire postfix expression
}



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
    // Test infix expression: (i1&~i2)|(~i1&i2)
    string infixExpression = "i1&i2";
    vector<bool> inputs = { true, false }; // Values for i1 and i2

    // Convert infix to postfix
    vector<string> postfixExpression = infixToPostfix(infixExpression);

    // Evaluate postfix expression
    bool result = evaluatePostfix(postfixExpression, inputs);

    // Display result
    cout << "Infix Expression: " << infixExpression << endl;
    cout << "Postfix Expression: ";
    for (const auto& token : postfixExpression) {
        cout << token << " ";
    }
    cout << endl;
    cout << "Result: " << result << endl;

    return 0;
}




