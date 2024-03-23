#ifndef Postfix_Functions_H
#define Postfix_Functions_H
#include <iostream>
#include <stack>
#include<vector>
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






bool evaluatePostfix(const vector<string>& postfix, unordered_map<string, pair<bool, int>>& map, vector<component>& gs, int i, int delay) {
    stack<bool> operands;
    int biggestinput = 0;
    for (const string& token : postfix) {
        if (isalpha(token[0])) {
            // If token is an input variable, fetch its value from the inputs vector
            int index = token[1] - '1'; // Assuming inputs are named as i1, i2, ..., and their values are provided in inputs vector
            operands.push(map[gs[i].ins[index]].first);
            biggestinput = max(biggestinput, map[gs[i].ins[index]].second);


        }
        else {

            if (token == "~") {
                // Negation operator
                if (!operands.empty()) {
                    bool operand = operands.top();
                    operands.pop();
                    operands.push(!operand);

                }
                else {
                    // Handle error: Missing operand
                    // This could be due to incorrect postfix expression
                    cout << "Error: Missing operand for negation operator '~'" << endl;
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










#endif 
