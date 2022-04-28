#include "varRegion.h"

varRegion::varRegion()
{
    initAlphabet();
}

void varRegion::initAlphabet()
{
    for (unsigned int i = 0; i < baseNum.size(); i++)
        decode[baseNum[i]] = i;
}

void varRegion::initVar(std::string name, number initValue = 0)
{
    varResolver[name] = (int)varStack.size();
    varStack.push_back(initValue);
}

void varRegion::setVar(std::string name, number value)
{
    if (name.size() == 0)
        log.THROW((std::string) "Varrible name not specified");
    if (varResolver.find(name) == varResolver.end())
    {
        initVar(name, value);
        return;
    }
    unsigned int varId = varResolver[name];
    if (varStack.size() < varId)
        log.THROW((std::string) "Internall error: cannot acces varriable");
    varStack[varId] = value;
}

number varRegion::getVar(std::string name)
{
    if (name.size() == 0 || varResolver.find(name) == varResolver.end())
    {
        std::string msg = "Varrible " + name + " is not avaliable";
        log.THROW(msg);
    }
    unsigned int varId = varResolver[name];
    if (varStack.size() < varId)
        log.THROW("Internall error: cannot acces varriable");
    return varStack[varId];
}

number varRegion::makeOperation(const number *a, const number *b, const char op)
{
    number ret(0);
    if (op == 0)
    {
        ret = *b;
        return ret;
    }
    switch (op)
    {
    case '+':
        ret = (*a) + (*b);
        break;
    case '-':
        ret = (*a) - (*b);
        break;
    case '*':
        ret = (*a) * (*b);
        break;
    case '/':
        ret = (*a) / (*b);
        break;
    case '^':
        ret = (*a) ^ (*b);
        break;
    case '&':
        ret = (*a) & (*b);
        break;
    case '|':
        ret = (*a) | (*b);
        break;
    case '<':
        ret = (*a) < (*b);
        break;
    case '>':
        ret = (*a) > (*b);
        break;
    case '=':
        ret = (*a) == (*b);
        break;
    default:
        std::string errop;
        errop.push_back(op);
        log.THROW("Operation " + errop + " is not defined");
        break;
    }
    return ret;
}

number varRegion::parseExpr(const std::string *expr, unsigned int *_it, unsigned int depth = 0)
{
    number result(0), _tmp(0);
    char bufer = 0;
    while (*_it < expr->size())
    {
        if (expr->at(*_it) == '#')
        {
            std::string varName = readVarName(expr, _it);
            _tmp = this->getVar(varName);
        }
        else if (expr->at(*_it) == '$')
            _tmp = readNumber(expr, _it);
        else if (expr->at(*_it) == '[')
        {
            (*_it)++;
            _tmp = parseExpr(expr, _it, depth + 1);
        }
        else
            log.THROW("Cannot parse expr");
        result = makeOperation(&result, &_tmp, bufer);
        if (*_it == expr->size())
            return result;
        if (expr->at(*_it) == ']')
        {
            if (depth == 0)
                log.THROW("Didn't expect ']' in expresion");
            (*_it)++;
            return result;
        }
        bufer = expr->at(*_it);
        (*_it)++;
    }
    log.THROW("Didn't expect end of expresion");
    return result;
}

std::string varRegion::readVarName(const std::string *expr, unsigned int *it)
{
    std::string resp;
    bool isPointer = false;
    if (expr->at(*it) == '#')
        (*it)++;
    if (expr->at(*it) == '*')
    {
        isPointer = true;
        (*it)++;
    }
    while (true)
    {
        if (expr->at(*it) == '#')
        {
            if (isPointer)
                resp = getVar(resp).toBase(64,&baseNum);
            (*it)++;
            return resp;
        }
        resp.push_back(expr->at(*it));
        (*it)++;
        if (*it == expr->size())
        {
            log.THROW("End of var name is not provided: reached end of text");
        }
    }
}

number varRegion::readNumber(const std::string *base64, unsigned int *it)
{
    if (base64->length() <= *it)
        log.THROW("parse64ToNum: provided string is empty");
    if (base64->at(*it) == '$')
        (*it)++;
    number ret(0), base = 64;

    unsigned int len = parseCharToBase(base64->at(*it), base).val();
    (*it)++;
    if (base64->length() < len + *it)
        log.THROW("parse64ToNum: too short string");
    for (; len > 0; len--, (*it)++)
    {
        ret = ret * base;
        ret = ret + parseCharToBase(base64->at(*it), 64);
    }
    return ret;
}

number varRegion::parseCharToBase(const char c, const number base)
{
    if (decode.find(c) == decode.end())
        log.THROW("Char is not a number");
    if (decode[c] > base)
        log.THROW("Char is not int base range");
    number num = decode[c];
    return num;
}