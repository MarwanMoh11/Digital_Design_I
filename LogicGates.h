#ifndef LOGIC_GATES_H
#define LOGIC_GATES_H



#include <iostream>
#include <vector>
using namespace std;

class LogicGates
{
private:
    int delay;
	int numInputs;
    string name;
public: 
	LogicGates();
	LogicGates(string, int, int);
	void setName(string);
    string getname() const;
    void setDelay(int);
    int getdelay() const;
	void setNuminputs(int);
    int getnuminputs() const;
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





#endif // LOGIC_GATES_H