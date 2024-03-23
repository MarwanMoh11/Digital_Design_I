#ifndef Postfix_Functions_H
#define Postfix_Functions_H
#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <unordered_map>
#include "component.h"
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
    stack<char> operators; // Stack to store operators
    vector<string> postfix; // Vector to store postfix expression
    string temp; // Temporary string to store operands or multi-digit numbers

    // Iterate through each character in the infix expression
    for (char ch : infix) {
        // If the character is an alphabet or digit, it's part of an operand
        if (isalpha(ch) || isdigit(ch)) {
            temp.push_back(ch); // Append to the temporary string
        }
        // If the character is an operator
        else {
            // If the temporary string is not empty, push it to the postfix expression
            if (!temp.empty()) {
                postfix.push_back(temp);
                temp.clear(); // Clear the temporary string
            }
            // If the character is an opening parenthesis
            if (ch == '(') {
                operators.push(ch); // Push it to the stack
            }
            // If the character is a closing parenthesis
            else if (ch == ')') {
                // Pop and push operators from the stack to postfix until '(' is encountered
                while (!operators.empty() && operators.top() != '(') {
                    postfix.push_back(charToString(operators.top()));
                    operators.pop();
                }
                operators.pop(); // Discard '('
            }
            // If the character is an operator
            else {
                // Pop and push operators from the stack to postfix with higher precedence
                while (!operators.empty() && precedence(ch) <= precedence(operators.top())) {
                    postfix.push_back(charToString(operators.top()));
                    operators.pop();
                }
                operators.push(ch); // Push the current operator to the stack
            }
        }
    }

    // If the temporary string is not empty, push it to the postfix expression
    if (!temp.empty()) {
        postfix.push_back(temp);
        temp.clear(); // Clear the temporary string
    }

    // Push any remaining operators from the stack to postfix
    while (!operators.empty()) {
        postfix.push_back(charToString(operators.top()));
        operators.pop();
    }

    return postfix; // Return the postfix expression
}






bool evaluatePostfix(const vector<string>& postfix, unordered_map<string, pair<bool, int>>& map, vector<component>& gs, int i, int delay, priority_queue<outputs>& pq) {
    stack<bool> operands; // Stack to store operands
    vector<int> inputTimes; // Vector to store input times
    set<int> uniqueDelays;// Set to store unique delays

    for (const string& token : postfix) {
        if (isalpha(token[0])) {
            int index = token[1] - '1'; // Get the index of the input we do -1 because the inputs are named i1, i2, etc...
            operands.push(map[gs[i].ins[index]].first);//push the value of the input to the stack
            inputTimes.push_back(map[gs[i].ins[index]].second);//push the delay of the input to the vector
        }
        else {
            if (token == "~") { // we pop the top of the stack and negate it
                if (!operands.empty()) {//if the stack is not empty
                    bool operand = operands.top();
                    operands.pop();
                    operands.push(!operand);
                }
                else {
                    cout << "Error: Missing operand for negation operator '~'" << endl;
                    return false;
                }
            }
            else {
                if (operands.size() < 2) { //if the stack has less than 2 operands which means that we store output value and delay
                    map[gs[i].out].first = operands.top();
                    // this whole for loop is to calculate the new delay and push it to the priority queue and make sure that we don't have duplicate delays for the same output
                    for (int inputTime : inputTimes) {
                        int newDelay = inputTime + delay; // Calculate the new delay
                        if (uniqueDelays.find(newDelay) == uniqueDelays.end()) { // Check if the delay is unique
                            map[gs[i].out].second = newDelay;//store the new delay
                            outputs temp(map[gs[i].out].second, map[gs[i].out].first, gs[i].out); // Create an output object
                            pq.push(temp);//push the output to the priority queue
                            uniqueDelays.insert(newDelay);//insert the delay to the set
                        }
                    }
                    return false;
                } //if the stack has 2 operands we pop the top 2 operands and apply the operator
                bool operand2 = operands.top();
                operands.pop();
                bool operand1 = operands.top();
                operands.pop();
                if (token == "&") {
                    operands.push(operand1 && operand2); //push the result of the operation to the stack
                }
                else if (token == "|") {
                    operands.push(operand1 || operand2); //push the result of the operation to the stack
                }
            }
        }
    }
    // This if statement is to check if the expression is invalid
    if (operands.size() != 1) {
        cerr << "Error: Invalid expression" << endl;
        return false;
    }

    return operands.top();
}









#endif 
