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
    vector<string> postfix = TruthTable::infixToPostfix(tokens);

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
    tt.evaluate();
    tt.printTable();

    for (auto &pair : operatorMap)
    {
        delete pair.second;
    }

    cout << "\nWould you like to save the truth table to a file? (Y/N): ";
    char choice;
    cin >> choice;
    cin.ignore(); // clear input buffer

    if (toupper(choice) == 'Y')
    {
        cout << "Enter filename (e.g., table.txt): ";
        string filename;
        getline(cin, filename);

        // build list of unique operators
        unordered_set<string> seen;
        vector<string> usedOps;
        for (const string &tok : tokens)
        {
            if (keywords.count(tok) && seen.count(tok) == 0)
            {
                usedOps.push_back(tok);
                seen.insert(tok);
            }
        }

        tt.saveToFile(filename, input, usedOps);
    }
    return 0;
}
