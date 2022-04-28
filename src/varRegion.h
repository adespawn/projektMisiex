#include "vector"
#include "string"
#include "map"

#include "number.h"
#include "logger.h"

#ifndef __VARREGION_C
#define __VARREGION_C

class varRegion
{
private:
    std::map<char, int> decode;
    std::vector<number> varStack;
    std::map<std::string, int> varResolver;
    logger log;

    void initAlphabet();
public:
    const std::string baseNum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    void initVar(std::string, number);
    void setVar(std::string, number);
    number getVar(std::string);
    number parseExpr(const std::string *, unsigned int *, unsigned int);
    std::string readVarName(const std::string *, unsigned int *);

    number makeOperation(const number *, const number *, const char);
    number readNumber(const std::string *, unsigned int *);
    number parseCharToBase(const char, const number);

    varRegion();
};

#endif