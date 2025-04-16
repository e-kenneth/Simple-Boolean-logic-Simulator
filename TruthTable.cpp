#include "TruthTable.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <fstream>
using namespace std;

vector<vector<bool>> TruthTable::generateInputs()
{
    vector<vector<bool>> inputs;
    for (int i = 0; i < 8; i++)
    {
        inputs.push_back({(bool)(i & 4), (bool)(i & 2), (bool)(i & 1)});
    }
    return inputs;
}

string TruthTable::generateIntermediateName(const string op, const string a, const string b)
{
    if (op == "NOT")
        return op + " " + a;
    return "(" + a + " " + op + " " + b + ")";
}

TruthTable::TruthTable(const vector<string> &postfixExpr, const map<string, BooleanOperator *> &opMap)
    : postfix(postfixExpr), operatorMap(opMap) {}

void TruthTable::evaluate()
{
    // Cleaning up previous data
    tableRows.clear();
    intermediateCols.clear();
    vector<vector<bool>> inputs = generateInputs();

    // for each A, B, C
    for (const auto &row : inputs)
    {
        bool A = row[0], B = row[1], C = row[2];
        // store current variable values
        map<string, bool> vars = {{"A", A}, {"B", B}, {"C", C}};
        // this is to simulate postfix evaluation
        vector<string> stack;
        // this is to store results after each operation is done eg. after A OR B is done, it will be stored here
        map<string, bool> tempResults;

        // Postfix evaluation
        // process each token in order, stored in postfix vector
        for (const string &token : postfix)
        {
            // If token is the variable, push to stack
            if (token == "A" || token == "B" || token == "C")
            {
                stack.push_back(token);
            }
            // unless, if its the not operator, get the last value from stack
            // eg C, check if it is a previously computed variable
            else if (token == "NOT")
            {
                string a = stack.back(); // get operand
                stack.pop_back();
                bool val = vars.count(a) ? vars[a] : tempResults[a]; // value of A or previous result
                // evaluating not operation
                bool result = operatorMap.at("NOT")->nevaluate(val);
                // generating label --> "NOT varname"
                string name = generateIntermediateName("NOT", a);
                stack.push_back(name);
                tempResults[name] = result; // tempresult holds these labels
                if (find(intermediateCols.begin(), intermediateCols.end(), name) == intermediateCols.end())
                    intermediateCols.push_back(name);
            }
            else
            {
                // pop 2 values
                string b = stack.back();
                stack.pop_back();
                string a = stack.back();
                stack.pop_back();
                bool valA, valB;
                // evaluate a and b
                if (vars.count(a))
                {
                     valA = vars[a];
                }
                else
                {
                     valA = tempResults[a];
                }
                if (vars.count(b))
                {
                     valB = vars[b];
                }
                else
                {
                     valB = tempResults[b];
                }
                bool result = operatorMap.at(token)->evaluate(valA, valB);
                // then label it
                string name = generateIntermediateName(token, a, b);
                stack.push_back(name);
                tempResults[name] = result;
                // this is if the intermediate result isn't alr on the list
                if (find(intermediateCols.begin(), intermediateCols.end(), name) == intermediateCols.end())
                    intermediateCols.push_back(name);
            }
        }
        // after a row is evaluated, push them to the vector tableRows
        vector<bool> rowData = {A, B, C};
        for (const string &col : intermediateCols)
        {
            rowData.push_back(tempResults[col]);
        }
        tableRows.push_back(rowData);
    }
}

void TruthTable::printTable()
{
    cout << "| A | B | C ";
    for (const string col : intermediateCols)
    {
        cout << "| " << col << " ";
    }
    cout << "|\n";

    cout << "|---|---|---";
    for (const string col : intermediateCols)
    {
        cout << "|" << string(col.length() + 2, '-');
    }
    cout << "|\n";
    // use pass by ref for efficiency
    for (const auto row : tableRows)
    {
        cout << "| " << row[0] << " | " << row[1] << " | " << row[2];
        for (int i = 3; i < row.size(); i++)
        {
            cout << " | "<< row[i] << " ";
        }
        cout << "|\n";
    }
}

void TruthTable::saveToFile(const string &filename, const string &originalExpression, const vector<string> &detectedOperators)
{
    ofstream out(filename);
    if (!out.is_open())
    {
        cerr << "Error: Could not open file: " << filename << endl;
        return;
    }

    out << "*** BOOLEAN EXPRESSION TRUTH TABLE ***\n\n";
    out << "Expression: " << originalExpression << "\n\n";

    out << "Operators Used:\n";
    for (const string &op : detectedOperators)
    {
        out << "- " << op << ": " << operatorMap.at(op)->explain() << "\n";
    }
    out << "\nTruth Table:\n";
    out << "| A | B | C ";
    for (const string &col : intermediateCols)
        out << "| " << col << " ";
    out << "|\n";

    out << "|---|---|---";
    for (const string &col : intermediateCols)
        out << "|" << string(col.length() + 2, '-');
    out << "|\n";

    for (const auto &row : tableRows)
    {
        out << "| " << row[0] << " | " << row[1] << " | " << row[2];
        for (size_t i = 3; i < row.size(); i++)
            out << " | " << setw(intermediateCols[i - 3].length()) << row[i] << " ";
        out << "|\n";
    }

    out.close();
    cout << "\nTruth table saved to '" << filename << "' successfully!\n";
}
