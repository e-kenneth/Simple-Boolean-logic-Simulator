#define BooleanOperator_H
#include <iostream>
#include <stdio.h>
using namespace std;

class BooleanOperator
{
public:
        virtual bool evaluate(bool a, bool b)
        {       
                // when i make this not pure virtual, other derived classes won't have to override it, seeing only the Not operator does not need it. 
                throw logic_error(getName() + " does not support binary evaluation.");
        }

        virtual bool nevaluate(bool a)
        {
                // when i make this not pure virtual, other derived classes won't have to override it, seeing only the Not operator needs it. 
                throw logic_error(getName() + " does not support unary evaluation.");
        }
        virtual string explain() = 0;
        virtual string getName() = 0;
        // destructor
        virtual ~BooleanOperator() = default;
};

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