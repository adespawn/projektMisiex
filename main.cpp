#include <bits/stdc++.h>
#define DEBUG 0
using namespace std;

const string baseNum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";

struct NUM;
class PROG;
class INS;
NUM getNum(int);

void THROW(string err, int sev = 0)
{
    switch (sev)
    {
    case 0:
        cout << "CRITICAL ERROR: EXITNG\n";
        cout << err << '\n';
        exit(1);
        break;
    case 1:
        cout << "CRITICAL INPUT ERROR: EXITNG\n";
        cout << err << '\n';
        exit(0);
        break;
    case 2:
        cout << "CRITICAL RUNTIME ERROR: EXITNG\n";
        cout << err << '\n';
        exit(0);
        break;
    default:
        cout << "UNKNOWN ERROR: EXITNG\n";
        cout << err << '\n';
        exit(1);
        break;
    }
}

struct NUM
{
    long long v;
    NUM operator+(const NUM b) const
    {
        NUM ret;
        ret.v = this->v + b.v;
        return ret;
    }
    NUM operator-(const NUM b) const
    {
        NUM ret;
        ret.v = this->v - b.v;
        return ret;
    }
    NUM operator*(const NUM b) const
    {
        NUM ret;
        ret.v = this->v * b.v;
        return ret;
    }
    NUM operator/(const NUM b) const
    {
        NUM ret;
        ret.v = this->v / b.v;
        return ret;
    }
    NUM operator^(const NUM b) const
    {
        NUM ret;
        ret.v = this->v ^ b.v;
        return ret;
    }
    NUM operator&(const NUM b) const
    {
        NUM ret;
        ret.v = this->v & b.v;
        return ret;
    }
    NUM operator%(const NUM b) const
    {
        NUM ret;
        ret.v = this->v % b.v;
        return ret;
    }
    NUM operator|(const NUM b) const
    {
        NUM ret;
        ret.v = this->v | b.v;
        return ret;
    }
    friend bool operator<(const NUM &a, const NUM &b)
    {
        return a.v < b.v;
    }
    friend bool operator>(const NUM &a, const NUM &b)
    {
        return b < a;
    }
    friend bool operator<=(const NUM &a, const NUM &b)
    {
        return !(a > b);
    }
    friend bool operator>=(const NUM &a, const NUM &b)
    {
        return !(a < b);
    }
    friend bool operator==(const NUM &a, const NUM &b)
    {
        return a.v == b.v;
    }
    friend bool operator!=(const NUM &a, const NUM &b)
    {
        return !(a == b);
    }
    NUM(long long v)
    {
        this->v = v;
    }
    NUM()
    {
        this->v = 0;
    }
    string toBase(NUM base)
    {
        if (base > getNum(64) || base < getNum(2))
        {
            THROW("requesting to convert to nonexisting base", 1);
        }
        string res;
        NUM me = *this;
        while (me > getNum(0))
        {
            res.push_back(baseNum[(me % base).v]);
            me = me / base;
        }
        reverse(res.begin(), res.end());
        if (res.length() == 0)
            res = "0";
        return res;
    }
};

NUM getNum(int num)
{
    NUM ret;
    ret.v = num;
    return ret;
}

class RTD
{

private:
    map<string, NUM> var;

public:
    RTD() {}
    NUM getVar(string);
    void setNumb(string, NUM);
    void log();
    ~RTD() {}
};

class exprHandler
{
private:
    RTD *runtimeDTA;
    map<char, int> decode;
    void initAlphabet()
    {
        for (int i = 0; i < baseNum.size(); i++)
        {
            decode[baseNum[i]] = i;
        }
    }

public:
    NUM parseExpr(string *, int *, int);
    NUM parse64ToNum(string *tmp)
    {
        int p = 0;
        return parse64ToNum(tmp, &p);
    }
    NUM parse64ToNum(string *, int *);
    NUM parseCharToBase(char, NUM);
    string readVarName(string *, int *);
    exprHandler()
    {
        initAlphabet();
    }
    exprHandler(RTD *runtimeDTA)
    {
        this->runtimeDTA = runtimeDTA;
        initAlphabet();
    }
};

NUM charToNum(char c)
{
    exprHandler expr;
    NUM retv = expr.parseCharToBase(c, 64);
    return retv;
}

NUM readNum(string *expr, int *it)
{
    if (expr->at(*it) == '$')
    {
        (*it)++;
    }
    exprHandler handler;
    NUM ret = handler.parse64ToNum(expr, it);
    return ret;
}

string exprHandler::readVarName(string *expr, int *it)
{
    string resp;
    bool isPointer = false;
    if (expr->at(*it) == '#')
    {
        (*it)++;
    }
    if (expr->at(*it) == '*')
    {
        isPointer = true;
        (*it)++;
    }
    while (true)
    {
        if (expr->at(*it) == '#')
        {
            (*it)++;
            if (isPointer)
            {
                resp = this->runtimeDTA->getVar(resp).toBase(64);
            }
            return resp;
        }
        charToNum(expr->at(*it));
        resp.push_back(expr->at(*it));
        (*it)++;
        if (*it == expr->size())
        {
            THROW("End of var name is not provided: reached end of text", 1);
        }
    }
}

NUM makeOperation(const NUM *a, const NUM *b, char op)
{
    NUM ret;
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
        string errop;
        errop.push_back(op);
        THROW("Operation " + errop + " is not defined", 2);
        break;
    }
    return ret;
}

NUM exprHandler::parseExpr(string *expr, int *it, int d)
{
    NUM res, temp;
    char bufOp = 0;
    while (*it < expr->size())
    {
        if (expr->at(*it) == '#')
        {
            string varName = readVarName(expr, it);
            temp = this->runtimeDTA->getVar(varName);
        }
        else if (expr->at(*it) == '$')
        {
            temp = readNum(expr, it);
        }
        else if (expr->at(*it) == '[')
        {
            (*it)++;
            temp = parseExpr(expr, it, d + 1);
        }
        else
        {
            THROW("Cannot parse expr", 2);
        }
        res = makeOperation(&res, &temp, bufOp);
        if (*it == expr->size())
        {
            return res;
        }
        if (expr->at(*it) == ']')
        {
            if (d == 0)
            {
                THROW("Didn't expect ']' in expresion", 2);
            }
            (*it)++;
            return res;
        }
        bufOp = expr->at(*it);
        (*it)++;
    }
    THROW("Didn't expected end of expresion");
    return res;
}

NUM exprHandler::parseCharToBase(char c, NUM base)
{
    if (decode.find(c) == decode.end())
    {
        THROW("Char is not a number", 1);
    }
    NUM num = decode.find(c)->second;
    if (num > base)
    {
        THROW("Char is not int base range", 1);
    }
    return num;
}

NUM exprHandler::parse64ToNum(string *base64, int *it)
{
    if (base64->length() - *it <= 0)
    {
        THROW("parse64ToNum: provided empty string", 1);
    }
    NUM ret;
    NUM len = parseCharToBase(base64->at(*it), getNum(64));
    if (base64->length() - *it < len.v + 1)
    {
        THROW("parse64ToNum: to short string", 1);
    }
    for (NUM i = getNum(0 + *it); i < len + *it; i = i + getNum(1))
    {
        ret = ret * getNum(64);
        ret = ret + parseCharToBase(base64->at(i.v + 1), getNum(64));
    }
    *it += 1 + len.v;
    return ret;
}

NUM RTD::getVar(string key)
{
    if (this->var.find(key) == this->var.end())
    {
        THROW("Atempting to acces nonexisting varrible", 1);
    }
    return var[key];
}

void RTD::setNumb(string key, NUM num)
{
    var[key] = num;
}

class INS
{
private:
    string insTXT, CMD;
    vector<string> argv;
    int parseIns();

public:
    INS(unsigned int *, const string *);
    ~INS() {}
    void log();
    void runCMD(PROG *);
};

class PROG
{
private:
    vector<INS> ik;

public:
    int mainParser(string);
    int programThread(int);
    stack<int> insStack;
    exprHandler handler;
    RTD data;
    PROG() {}
    ~PROG() {}
};

INS::INS(unsigned int *insC, const string *mainC)
{
    for (*insC; *insC < mainC->size(); (*insC)++)
    {
        char tm = mainC->at(*insC);
        if (tm == '~')
        {
            (*insC)++;
            this->parseIns();
            return;
        }
        insTXT.push_back(tm);
    }
    THROW("Reached EOF without \"~\" character", 1);
}

int INS::parseIns()
{
    bool isCMD = 1;
    string currArg;
    for (auto i : insTXT)
    {
        if (i == ':')
        {
            if (isCMD)
            {
                CMD = currArg;
                isCMD = 0;
            }
            else
            {
                argv.push_back(currArg);
            }
            currArg.clear();
        }
        else
        {
            currArg.push_back(i);
        }
    }
    if (isCMD)
    {
        CMD = currArg;
    }
    else
    {
        argv.push_back(currArg);
    }
    return 0;
}

void INS::runCMD(PROG *mainPROG)
{
    int curIns = mainPROG->insStack.top();
    mainPROG->insStack.pop();
    int pos = 0;
    if (CMD == "@")
    {
        NUM tmp = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        curIns += tmp.v;

        if (DEBUG)
            cout << "skiping " << tmp.v << " instructions\n";
    }
    if (CMD == "@I")
    {
        NUM cond = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        pos = 0;
        NUM tmp = mainPROG->handler.parseExpr(&argv[1], &pos, argv[1].size());
        if (cond > getNum(0))
        {
            curIns += tmp.v + 1;
            if (DEBUG)
                cout << "skiping " << tmp.v << " instructions(cond)\n";
        }
        else
        {
            if (DEBUG)
                cout << "skiping skip\n";
        }
    }
    else if (CMD == "@BI")
    {
        NUM cond = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        pos = 0;
        NUM tmp = mainPROG->handler.parseExpr(&argv[1], &pos, argv[1].size());
        if (curIns < tmp.v)
            THROW("Cannot move back to negative instruction", 1);
        if (cond > getNum(0))
        {
            curIns -= tmp.v + 1;
            if (DEBUG)
                cout << "going back " << tmp.v << " instructions\n";
        }
        else
        {
            if (DEBUG)
                cout << "skiping skip\n";
        }
    }
    else if (CMD == "@B")
    {
        NUM tmp = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        if (curIns < tmp.v)
            THROW("Cannot move back to negative instruction", 1);
        curIns -= tmp.v;

        if (DEBUG)
            cout << "going back " << tmp.v << " instructions\n";
    }
    else if (CMD == "?R")
    {
        string varName = mainPROG->handler.readVarName(&argv[0], &pos);
        string input;
        cin >> input;
        NUM readNum = mainPROG->handler.parse64ToNum(&input);
        mainPROG->data.setNumb(varName, readNum);
        if (DEBUG)
            cout << "Set " << varName << " to " << mainPROG->data.getVar(varName).v << '\n';
    }
    else if (CMD == "?P")
    {
        NUM num = mainPROG->data.getVar(mainPROG->handler.readVarName(&argv[0], &pos));
        cout << num.toBase(64) << '\n';
    }
    else if (CMD == "?C")
    {
        NUM num = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        if (false)
        {
            THROW("Cannot start thread from nonexisting instruction", 1);
        }
        mainPROG->insStack.push(curIns);
        mainPROG->programThread(num.v);
        return;
    }
    else if (CMD == "?RE")
    {
        mainPROG->insStack.push(-10);
        return;
    }
    else if (CMD.size() > 0 && CMD[0] == '#')
    {
        string varName = mainPROG->handler.readVarName(&CMD, &pos);
        pos = 0;
        NUM newVal = mainPROG->handler.parseExpr(&argv[0], &pos, argv[0].size());
        mainPROG->data.setNumb(varName, newVal);
        if (DEBUG)
            cout << "setting var " << varName << " to " << mainPROG->data.getVar(varName).v << '\n';
    }
    else
    {
    }

    mainPROG->insStack.push(curIns);
}

void INS::log()
{
    cout << CMD << '\n';
    for (auto i : argv)
    {
        cout << i << ' ';
    }
    cout << '\n';
}

int PROG::mainParser(string Txt)
{
    exprHandler tmp(&data);
    handler = tmp;
    unsigned int codeIt = 0;
    while (codeIt != Txt.size())
    {
        INS newINS(&codeIt, &Txt);
        ik.push_back(newINS);
    }
    try
    {
        this->programThread(0);
    }
    catch (...)
    {
        THROW("Unknow error in threads", 2);
    }
    return 0;
}

int PROG::programThread(int ins)
{
    this->insStack.push(ins);
    if (DEBUG)
        cout << "STACK " << insStack.size() << "\n";
    while (ins < ik.size() && ins >= 0)
    {
        if (DEBUG)
            cout << "INS " << ins << '\n';
        ik.at(ins).runCMD(this);
        ins = this->insStack.top();
        this->insStack.pop();
        ins++;
        this->insStack.push(ins);
    }
    if (DEBUG)
        cout << "Exiting STACK " << insStack.size() << "\n";
    this->insStack.pop();
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        THROW("Provide file name to run");
    }
    try
    {
        ifstream file(argv[1]);
        string code;
        file >> code;
        PROG prog;
        prog.mainParser(code);
    }
    catch (const std::exception &e)
    {
        THROW(e.what());
    }
}