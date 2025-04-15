#define BooleanOperator_H
#include <iostream>
#include <stdio.h>
using namespace std;

class BooleanOperator
{
public:
        virtual bool evaluate(bool a, bool b)
        {       
                // everything except the Not operator will derive this. 
                throw logic_error(getName() + " does not support binary evaluation.");
        }

        virtual bool nevaluate(bool a)
        {
                // only the Not operator will derive this. 
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