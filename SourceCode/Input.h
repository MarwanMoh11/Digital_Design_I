#ifndef Input_H
#define Input_H
#include <iostream>
#include <vector>
using namespace std;

struct Input {
	string name;
	bool value;
	int time_stamp;

	Input() {}
	Input(string n, bool v, int d)
	{
		name = n;
		value = v;
		time_stamp = d;
}
	bool operator<(const Input& other) const {
		// This defines the order in the priority queue based on priority field
		return time_stamp > other.time_stamp;
	}

};



#endif // !Input_H
