#include "vector"
#include "string"
#include "map"
#include "stack"

#include "number.h"
#include "logger.h"
#include "instruction.h"
#include "varRegion.h"

#ifndef __RUNTIME_C
#define __RUNTIME_C
struct threadS
{
    unsigned int insPoz;
};

class runtime
{
private:
    std::string runtimeCode;
    std::vector<instruction> insOrder;
    std::stack<threadS> threadStack;
    int programThread(unsigned int);
    logger log;
    varRegion globalRegion;

public:
    runtime(const std::string &);
    ~runtime();
    int run();
};

#endif