#include "BooleanOperator.h"
#include <iostream>
using namespace std;
#include <vector>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
// This function "tokenizes" the input and seperate them into keywords, which then can be matched with my list of keywords. 
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    for (auto c : input) {
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '(' || c == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));
        } else {
            token += c;
        }
    }
    if (!token.empty())
        tokens.push_back(token);
    return tokens;
}
int main()
{
    // sample input
    // string input = "(A AND B) OR (NOT C)";
    string input;
    // said list of keywords. 
    unordered_set<string> keywords = {"AND", "OR", "NOT", "NAND", "NOR", "XOR"};
    // creating new objects of operators
    map<string, BooleanOperator*> operatorMap = {
        {"AND", new AndOperator()},
        {"OR", new OrOperator()},
        {"NOT", new NotOperator()},
        {"NAND", new NandOperator()},
        {"NOR", new NorOperator()},
        {"XOR", new XorOperator()}
    };

    cout <<"*** BOOLEAN TRUTH TABLE SIMULATOR ***"<<endl;
    cout<<"Enter Boolean Expression (max 3 ops, vars A, B, C):"<<endl;

    getline(cin, input);
    cout<<"Input: "<<input<<endl;

    // tokenizing input

    vector<string> tokens = tokenize(input);
    vector<string> foundKeywords;
    for (const string& token : tokens) {
        if (keywords.count(token)) {
            foundKeywords.push_back(token);
        }
    }

    cout<<"Operators detected and explained: "<<endl;
    for (const string& op : foundKeywords)
    {
        cout<<"- "<<operatorMap[op]->getName() << " : "<< operatorMap[op]->explain()<<endl; 
    }
    

    // delete the created operator objects 
    
    for (auto& pair : operatorMap) {
        delete pair.second;
    }
}