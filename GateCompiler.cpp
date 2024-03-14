#include "GateCompiler.h"
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>


using namespace std;

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
    return string(1, c);
}
vector<string> infixToPostfix(const string& infix) {
    stack<char> operators;
    vector<string> postfix;
    string temp;
    for (char ch : infix) {
        {
            if (isalpha(ch))
            {
                while (isalpha(ch))
                {
                    temp.push_back(ch);
                    ch++;
                }
                postfix.push_back(temp);
            }
            else if (ch == '(') {
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

        while (!operators.empty()) {
            postfix.push_back(charToString(operators.top()));
            operators.pop();
        }

        return postfix;
    }
}

    bool evaluatePostfix( vector<string>postfix, unordered_map<string, bool> inputs, vector<string>ins) {
        stack<bool> operands;
        unordered_map<string, string> map;

        int j = 0; // Move this initialization outside the loop

        for (int i = 0; i < postfix.size(); i++) {
            if (isalpha(postfix[i].at(0))) {
                map[postfix[i]] = ins[j++];
            }
        }

        for (int i = 0; i < postfix.size(); i++) {
            if (isalpha(postfix[i].at(0))) {
                operands.push(inputs[postfix[i]]); // Push the boolean value, not the pair
            }
            else if (postfix[i] == "&" || postfix[i] == "|") { // Missing closing parenthesis after this condition
                char ch = postfix[i][0]; // Extract the character
                bool operand2 = operands.top();
                operands.pop();
                bool operand1 = operands.top();
                operands.pop();
                postfix[i];

                if (postfix[i] == "&") {
                    operands.push(operand1 && operand2);
                }
                else if (postfix[i] == "|") {
                    operands.push(operand1 || operand2);
                }

            }
            else if (postfix[i] == "!") { // Add missing condition
                bool operand1 = operands.top();
                operands.pop();
                operands.push(!operand1);
            }
        }
        return operands.top();
    }


