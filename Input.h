#ifndef Input_H
#define Input_H
#include <iostream>
#include <vector>
using namespace std;

struct Input {
	string name;
	vector<pair<bool, int>> values;
	int used=0;


	Input() {}
	Input(string n, bool v, int d)
	{
		name = n;
		values.push_back({ v,d });
}


};



#endif // !Input_H
