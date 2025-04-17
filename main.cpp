#include <fstream>
#include "BooleanOperator.h"
#include "TruthTable.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <algorithm>
using namespace std;
vector<string> infixToPostfix(const vector<string> tokens)
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
vector<string> tokenize(const string &input)
{
    vector<string> tokens;
    string token;
    for (char c : input)
    {
        if (isspace(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (c == '(' || c == ')')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));
        }
        else
        {
            token += c;
        }
    }
    if (!token.empty())
        tokens.push_back(token);
    return tokens;
}

int main()
{
    string input;
    unordered_set<string> keywords = {"AND", "OR", "NOT", "NAND", "NOR", "XOR"};
    map<string, BooleanOperator *> operatorMap = {
        {"AND", new AndOperator()},
        {"OR", new OrOperator()},
        {"NOT", new NotOperator()},
        {"NAND", new NandOperator()},
        {"NOR", new NorOperator()},
        {"XOR", new XorOperator()}};

    cout << "*** BOOLEAN TRUTH TABLE SIMULATOR ***\n";
    cout << "Enter Boolean Expression (max 3 ops, vars A, B, C):\n";
    getline(cin, input);

    cout << "Input: " << input << endl;

    vector<string> tokens = tokenize(input);
    vector<string> postfix = infixToPostfix(tokens);

    cout << "Postfix Expression: ";
    for (const string &tok : postfix)
        cout << tok << " ";
    cout << endl;

    vector<string> foundKeywords;
    for (const string &token : tokens)
    {
        if (keywords.count(token))
        {
            foundKeywords.push_back(token);
        }
    }

    cout << "\nOperators detected and explained:\n";
    for (const string &op : foundKeywords)
    {
        cout << "- " << operatorMap[op]->getName() << " : " << operatorMap[op]->explain() << endl;
    }

    TruthTable tt(postfix, operatorMap);
    cout << "[DEBUG] calling evaluate()\n";
    tt.evaluate();
    cout << "[DEBUG] evaluate() completed\n";
    tt.printTable();

    cout << "\nWould you like to save the truth table to a file? (Y/N): ";
    char choice;
    cin >> choice;
    cin.ignore(); // clear input buffer

    if (toupper(choice) == 'Y')
    {
        cout << "Enter filename (e.g., table.txt): ";
        string filename;
        getline(cin, filename);

        unordered_set<string> seen;
        vector<string> usedOps;

        for (const string &tok : tokens)
        {
            // check if each operators are being stored
            cout << tok << endl;
            if (keywords.count(tok) > 0 && seen.count(tok) == 0)
            {
                usedOps.push_back(tok);
                seen.insert(tok);
            }
        }

        for (const string &testtokens : usedOps)
        {
            cout << testtokens << endl;
        }

        tt.saveToFile(filename, input, usedOps);
    }
    // clear operatormap contents
    for (auto &pair : operatorMap)
    {
        delete pair.second;
    }

    // testSimpleFileWrite();
}
