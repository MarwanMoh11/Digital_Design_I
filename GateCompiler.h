#ifndef GateCompiler_H
#define GateCompiler_H


class GateCompiler
{
public:
	int precedence(char);
	int infixToPostfix(const string&, unordered_map<string, pair<bool, int>>&, vector <string> &);
};

#endif