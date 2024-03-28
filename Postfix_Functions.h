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

// Function to calculate precedence of logic operators
int precedence(char op) {
    if (op == '~')
        return 3;
    if (op == '&')
        return 2;
    if (op == '|')
        return 1;
    return 0;
}

// Function to convert char to string
string charToString(char c) {
    return string(1, c); // Converts a character into a string of size 1
}

vector<string> infixToPostfix(const string& infix) {
    stack<char> operators;
    vector<string> postfix;
    string temp;

    for (char ch : infix) {
        if (ch == ' ')
            continue;
        else if (isalpha(ch) || isdigit(ch)) {
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
                while (!operators.empty() && precedence(ch) < precedence(operators.top())) {
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



// Function to evaluate postfix expression
bool evaluatePostfix(const vector<string>& postfix, const vector<bool>& inputs) {
    stack<bool> operands;

    for (const string& token : postfix) {
        if (isalpha(token[0])) {
            // If token is an input variable, fetch its value from the inputs vector
            int index = stoi(token.substr(1)) - 1; // Extract the numeric part of the input variable name
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









#endif 
