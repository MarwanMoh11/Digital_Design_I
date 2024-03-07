#include <iostream>
#include <vector>
using namespace std;

class LogicGates
{
private:
	string name;
	int delay;
	int numInputs;
public: 
	LogicGates();
	LogicGates(string, int, int);
	bool AND(vector<bool> x);
	bool OR(vector <bool>);
	bool NAND(vector < bool>);
	bool NOR(vector < bool>);
	bool Buffer(bool);
	bool NOT(bool);
	bool XOR(vector < bool>);
	bool XNOR(vector < bool>);
	~LogicGates();

};

