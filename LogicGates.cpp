//#include "LogicGates.h"
//
//LogicGates::LogicGates()
//{
//}
//
//LogicGates::LogicGates(string n, int num=1, int d=0)
//{
//    delay = d;
//    numInputs = num;
//    name = n;
//}
//
//void LogicGates::setName(string n)
//{
//    name = n;
//}
//
//void LogicGates::setDelay(int d)
//{
//    delay = d;
//}
//
//void LogicGates::setNuminputs(int in)
//{
//    numInputs = in;
//}
//
//
//bool LogicGates::AND(vector<bool> inputs)
//{
//    for (auto i : inputs)
//    {
//        if (!i) return false;
//   }
//    
//    return true;
//}
//
//bool LogicGates::OR(vector<bool> inputs)
//{
//    for (auto i : inputs)
//    {
//        if (i) return true;
//    }
//
//    return false;
//}
//
//bool LogicGates::NAND(vector<bool> inputs)
//{
//    for (auto i : inputs)
//    {
//        if (!i) return true;
//    }
//
//    return false;
//}
//
//bool LogicGates::NOR(vector<bool> inputs)
//{
//    for (auto i : inputs)
//    {
//        if (i) return false;
//    }
//
//    return true;
//}
//
//bool LogicGates::Buffer(bool n)
//{
//    return n;
//}
//
//bool LogicGates::NOT(bool n)
//{
//    return !n;
//}
//
//bool LogicGates::XOR(vector<bool> inputs)
//{
//    int count = 0;
//    for (auto i : inputs)
//    {
//        if (i) count++;
//    }
//
//    if (count == 1) return true;
//    return false;
//}
//
//bool LogicGates::XNOR(vector<bool> inputs)
//{
//    int count = 0;
//    for (auto i : inputs)
//    {
//        if (i) count++;
//    }
//
//    if (count == 1) return false;
//    return true;
//}
//
//LogicGates::~LogicGates()
//{
//}
//
//string LogicGates::getname() const {
//    return name;
//}
//
//int LogicGates::getdelay() const {
//    return delay;
//}
//
//int LogicGates::getnuminputs() const {
//    return numInputs;
//}
