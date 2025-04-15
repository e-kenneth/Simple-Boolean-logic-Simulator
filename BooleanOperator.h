#define BooleanOperator_H
#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;

class BooleanOperator
{
public:
        // everything except the Not operator will derive this.
        virtual bool evaluate(bool a, bool b)
        {
                //  Error Handling
                throw logic_error(getName() + " does not support binary evaluation.");
        }
        // only the Not operator will derive this.
        virtual bool nevaluate(bool a)
        {
                //  Error Handling
                throw logic_error(getName() + " does not support unary evaluation.");
        }
        // pure virtual function, each operator will derive from it.
        virtual string explain() = 0;
        virtual string getName() = 0;
        // default destructor
        virtual ~BooleanOperator() = default;
};
// deriving each operator from base class boolean operator.
class AndOperator : public BooleanOperator
{
public:
        bool evaluate(bool a, bool b) override;
        string explain() override;
        string getName() override;
};
class OrOperator : public BooleanOperator
{
        bool evaluate(bool a, bool b) override;
        string explain() override;
        string getName() override;
};
class NotOperator : public BooleanOperator
{
        bool nevaluate(bool a) override;
        string explain() override;
        string getName() override;
};
class NandOperator : public BooleanOperator
{
        bool evaluate(bool a, bool b) override;
        string explain() override;
        string getName() override;
};
class NorOperator : public BooleanOperator
{
        bool evaluate(bool a, bool b) override;
        string explain() override;
        string getName() override;
};
class XorOperator : public BooleanOperator
{
        bool evaluate(bool a, bool b) override;
        string explain() override;
        string getName() override;
};

// BooleanOperator::~BooleanOperator()
// {
//         // Nothing to clean up (unless needed)
// }