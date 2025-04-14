#include "BooleanOperator.h"
// AND
bool AndOperator::evaluate(bool a, bool b) {
    return a && b;
}
string AndOperator::explain(){
    return "The AND operator returns true Only if both a and b are true";
}
string AndOperator::getName(){
    return "AND";
}
// OR
bool OrOperator::evaluate (bool a, bool b) {
    return a || b;
}
string OrOperator::explain(){
    return "The OR operator returns true if a or b is true";
}
string OrOperator::getName(){
    return "OR";
}
// NOT
bool NotOperator::nevaluate(bool a){
    return !a;
}
string NotOperator::explain(){
    return "The NOT operator inverts the variable's value, from true to false and vice versa";
}
string NotOperator::getName(){
    return "NOT";
}
// NAND
bool NandOperator::evaluate(bool a, bool b){
    return !(a && b);
}
string NandOperator::explain(){
    return"The NAND operator outputs false only when both inputs are true, and true otherwise.";
}
string NandOperator::getName(){
    return "NAND";
}
// NOR 
bool NorOperator::evaluate(bool a, bool b){
    return !(a || b);
}
string NorOperator::explain(){
    return "The NOR operator outputs the inverse of whatever OR results in.";
}
string NorOperator::getName(){
    return "NOR";
}
// XOR
bool XorOperator::evaluate(bool a, bool b){
    return a ^ b;
}
string XorOperator::explain(){
    return "The XOR operator outputs true if exactly one of the inputs is true.";
}
string XorOperator::getName(){
    return "XOR";
}
// BooleanOperator::~BooleanOperator() {

// }