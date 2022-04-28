#include "instruction.h"

instruction::instruction(unsigned int *codePos, const std::string *code)
{
    bool _tmp = 0;
    for (; *codePos < code->size(); (*codePos)++)
    {
        char tm = code->at(*codePos);
        if (tm == '~')
        {
            (*codePos)++;
            _tmp = 1;
            break;
        }
        insTXT.push_back(tm);
    }
    if (!_tmp)
        log.THROW("Reached EOF without \"~\" character");
    bool isCMD = 1;
    std::string currArg;
    for (auto i : insTXT)
    {
        if (i == ':')
        {
            if (isCMD)
            {
                ins = currArg;
                isCMD = 0;
            }
            else
                argv.push_back(currArg);
            currArg.clear();
        }
        else
            currArg.push_back(i);
    }
    if (isCMD)
        ins = currArg;
    else
        argv.push_back(currArg);
}

instruction::~instruction()
{
}

insReturn::insReturn(typeE::type retType, int val = 0)
{
    status = retType;
    if (status == typeE::jump || status == typeE::call)
        pos = val;
    else if (status == typeE::exit)
        statusCode = val;
}

insReturn instruction::runIns(varRegion *region)
{
    unsigned int pos = 0;
    // Jump instruction
    if (ins == "@")
    {
        number tmp = region->parseExpr(&argv[0], &pos, argv[0].size());
        insReturn ret(typeE::jump, (int)tmp);
    }
    // Conditional jump instruction
    else if (ins == "@I")
    {
        number cond = region->parseExpr(&argv[0], &pos, argv[0].size());
        pos = 0;
        number tmp = region->parseExpr(&argv[1], &pos, argv[1].size());
        if (cond != 0)
        {
            insReturn ret(typeE::jump, (int)tmp);
        }
    }
    // Back jump instruction
    if (ins == "@B")
    {
        number tmp = region->parseExpr(&argv[0], &pos, argv[0].size());
        insReturn ret(typeE::jump, (int)-tmp);
    }
    // Conditional back jump instruction
    else if (ins == "@BI")
    {
        number cond = region->parseExpr(&argv[0], &pos, argv[0].size());
        pos = 0;
        number tmp = region->parseExpr(&argv[1], &pos, argv[1].size());
        if (cond != 0)
        {
            insReturn ret(typeE::jump, (int)-tmp);
        }
    }
    // Read varriable from input
    else if (ins == "?R")
    {
        std::string varName = region->readVarName(&argv[0], &pos), input;
        std::cin >> input;
        pos = 0;
        number readNum = region->readNumber(&input, &pos);
        region->setVar(varName, readNum);
    }
    // Print varriable to output
    else if (ins == "?P")
    {
        number num = region->parseExpr(&argv[0], &pos, 0);
        std::string out = num.toBase(64, &(region->baseNum));
        std::cout << "$" << region->baseNum[out.size()] << out << '\n';
    }
    // Print varriable to output in given base
    else if (ins == "?PB")
    {
        number num = region->parseExpr(&argv[0], &pos, 0);
        pos = 0;
        number base = region->parseExpr(&argv[1], &pos, 0);
        std::cout << num.toBase(base, &region->baseNum) << '\n';
    }
    // Call instruction
    else if (ins == "?C")
    {
        number tmp = region->parseExpr(&argv[0], &pos, argv[0].size());
        insReturn ret(typeE::call, (int)tmp);
        return ret;
    }
    // Exit call
    else if (ins == "?RE")
    {
        insReturn ret(typeE::exit);
        return ret;
    }
    //
    else if (ins == "\\")
    {
        if (argv.size() == 0 || (int)region->parseExpr(&argv[0], &pos, argv[0].size()) == 0)
        {
            insReturn ret(typeE::jump, closing);
            return ret;
        }
    }

    // Set varriable
    else if (ins.size() > 0 && ins[0] == '#')
    {
        std::string varName = region->readVarName(&ins, &pos);
        pos = 0;
        number newVal = region->parseExpr(&argv[0], &pos, argv[0].size());
        region->setVar(varName, newVal);
    }

    insReturn ret(typeE::ok);
    return ret;
}