#include "runtime.h"

runtime::runtime(const std::string &code)
{
    runtimeCode = code;
    unsigned int _it = 0;
    std::stack<unsigned int> opening;
    while (_it != runtimeCode.size())
    {
        if (code.at(_it) == '\\')
            opening.push(insOrder.size());
        if (code.at(_it) == '/')
        {
            if (opening.size() == 0)
                log.THROW("Cannot match opening bracket");
            insOrder[opening.top()].setBracket(insOrder.size() - opening.top());
            opening.pop();
        }
        instruction newIns(&_it, &runtimeCode);
        insOrder.push_back(newIns);
    }
    if (opening.size())
        log.THROW("No closing bracket found");
}

runtime::~runtime()
{
}

int runtime::programThread(unsigned int startIns = 0)
{
    threadS thread;
    thread.insPoz = startIns;
    for (; thread.insPoz < insOrder.size(); thread.insPoz++)
    {
        const insReturn _return = insOrder[thread.insPoz].runIns(&globalRegion);
        if (_return.status == typeE::exit)
            return _return.statusCode;
        if (_return.status == typeE::call)
        {
            threadStack.push(thread);
            if ((unsigned)_return.pos > insOrder.size())
                log.THROW("Cannot call nonexisting instruction");
            this->programThread(_return.pos);
            thread = threadStack.top();
            threadStack.pop();
        }
        if (_return.status == typeE::jump)
        {
            if (_return.pos > (int)thread.insPoz)
                log.THROW("Cannot jump to negative instruction");
            thread.insPoz += _return.pos;
        }
    }

    return 0;
}

int runtime::run()
{
    return programThread();
}