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

string TruthTable::generateIntermediateName(const string &op, const string &a, const string &b)
{
    if (op == "NOT")
        return op + " " + a;
    return "(" + a + " " + op + " " + b + ")";
}

TruthTable::TruthTable(const vector<string> &postfixExpr, const map<string, BooleanOperator *> &opMap)
    : postfix(postfixExpr), operatorMap(opMap) {}

void TruthTable::evaluate()
{
    tableRows.clear();
    intermediateCols.clear();
    vector<vector<bool>> inputs = generateInputs();

    for (const auto &row : inputs)
    {
        bool A = row[0], B = row[1], C = row[2];
        map<string, bool> vars = {{"A", A}, {"B", B}, {"C", C}};
        vector<string> stack;
        map<string, bool> tempResults;

        for (const string &token : postfix)
        {
            if (token == "A" || token == "B" || token == "C")
            {
                stack.push_back(token);
            }
            else if (token == "NOT")
            {
                string a = stack.back();
                stack.pop_back();
                bool val = vars.count(a) ? vars[a] : tempResults[a];
                bool result = operatorMap.at("NOT")->nevaluate(val);
                string name = generateIntermediateName("NOT", a);
                stack.push_back(name);
                tempResults[name] = result;
                if (find(intermediateCols.begin(), intermediateCols.end(), name) == intermediateCols.end())
                    intermediateCols.push_back(name);
            }
            else
            {
                string b = stack.back();
                stack.pop_back();
                string a = stack.back();
                stack.pop_back();
                bool valA = vars.count(a) ? vars[a] : tempResults[a];
                bool valB = vars.count(b) ? vars[b] : tempResults[b];
                bool result = operatorMap.at(token)->evaluate(valA, valB);
                string name = generateIntermediateName(token, a, b);
                stack.push_back(name);
                tempResults[name] = result;
                if (find(intermediateCols.begin(), intermediateCols.end(), name) == intermediateCols.end())
                    intermediateCols.push_back(name);
            }
        }

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
    for (const string &col : intermediateCols)
    {
        cout << "| " << col << " ";
    }
    cout << "|\n";

    cout << "|---|---|---";
    for (const string &col : intermediateCols)
    {
        cout << "|" << string(col.length() + 2, '-');
    }
    cout << "|\n";

    for (const auto &row : tableRows)
    {
        cout << "| " << row[0] << " | " << row[1] << " | " << row[2];
        for (size_t i = 3; i < row.size(); i++)
        {
            cout << " | " << setw(intermediateCols[i - 3].length()) << row[i] << " ";
        }
        cout << "|\n";
    }
}

vector<string> TruthTable::infixToPostfix(const vector<string> &tokens)
{
    vector<string> output;
    vector<string> opStack;

    map<string, int> precedence = {
        {"NOT", 3}, {"AND", 2}, {"NAND", 2}, {"OR", 1}, {"NOR", 1}, {"XOR", 1}};

    unordered_set<string> operators = {"NOT", "AND", "OR", "NAND", "NOR", "XOR"};
    unordered_set<string> rightAssociative = {"NOT"};

    for (const string &token : tokens)
    {
        if (token == "(")
        {
            opStack.push_back(token);
        }
        else if (token == ")")
        {
            while (!opStack.empty() && opStack.back() != "(")
            {
                output.push_back(opStack.back());
                opStack.pop_back();
            }
            if (!opStack.empty())
                opStack.pop_back(); // pop "("
        }
        else if (operators.count(token))
        {
            while (!opStack.empty() && operators.count(opStack.back()))
            {
                string top = opStack.back();
                if ((rightAssociative.count(token) == 0 && precedence[token] <= precedence[top]) ||
                    (rightAssociative.count(token) == 1 && precedence[token] < precedence[top]))
                {
                    output.push_back(top);
                    opStack.pop_back();
                }
                else
                    break;
            }
            opStack.push_back(token);
        }
        else
        {
            output.push_back(token); // Variable
        }
    }

    while (!opStack.empty())
    {
        output.push_back(opStack.back());
        opStack.pop_back();
    }

    return output;
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
