#include "string"
#include "vector"
#include "stack"
#include "iostream"

#include "logger.h"
#include "varRegion.h"

#ifndef __INSTRCTION_C
#define __INSTRCTION_C

namespace typeE
{
    enum type
    {
        ok,
        exit,
        call,
        jump
    };

}

struct insReturn
{
    typeE::type status;
    int statusCode;
    int pos;
    insReturn(typeE::type, int);
};

class instruction
{
private:
    std::string insTXT, ins;
    std::vector<std::string> argv;
    logger log;
    int closing;

public:
    void setBracket(int a) { closing = a; }
    insReturn runIns(varRegion *);
    instruction(unsigned int *, const std::string *);
    ~instruction();
};

class bracket : public instruction
{
private:
public:
    void setBracket(int);
    bracket(unsigned int *a, const std::string *b) : instruction(a, b) {}
};

#endif