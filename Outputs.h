#ifndef Outputs_H
#define Outputs_H
#include <iostream>
using namespace std;


struct outputs {

    outputs(int x, string y, bool z) {
        time_stamp_ps = x;
        input = y;
        logic_value = z;
    }

    bool operator<(const outputs& other) const {
        // This defines the order in the priority queue based on priority field
        return time_stamp_ps > other.time_stamp_ps;
    }

    int time_stamp_ps;
    string input;
    bool logic_value;
};


#endif 