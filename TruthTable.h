#ifndef TRUTH_TABLE_H
#define TRUTH_TABLE_H
#include "BooleanOperator.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <set>

using namespace std;

class TruthTable
{
private:
    vector<string> postfix;
    map<string, BooleanOperator*> operatorMap;
    // vector function to generate all combinations (A = 0, B = 0, C = 1) etc
    vector<vector<bool>> generateInputs();

    vector<string> intermediateCols;
    vector<vector<bool>> tableRows;

    // int tempCounter = 1;
    string generateIntermediateName(const string op, const string a, const string b = "");

public:
    // constructor
    TruthTable(const vector<string> &postfixExpr, const map<string, BooleanOperator *> &opMap);
    void saveToFile(const string& filename, const string& originalExpression, const vector<string>& detectedOperators);
    void evaluate();
    void printTable();
    static vector<string> infixToPostfix(const vector<string> tokens);
};
#endif