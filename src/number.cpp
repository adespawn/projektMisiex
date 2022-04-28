#include "number.h"

number::number(long long newValue = 0)
{
    value = newValue;
}

number::~number()
{
}

number number::operator+(const number b) const
{
    number ret;
    ret.value = value + b.value;
    return ret;
}

number number::operator-(const number b) const
{
    number ret;
    ret.value = value - b.value;
    return ret;
}

number number::operator*(const number b) const
{
    number ret;
    ret.value = value * b.value;
    return ret;
}

number number::operator/(const number b) const
{
    number ret;
    ret.value = value / b.value;
    return ret;
}

number number::operator=(const long long b)
{
    number ret(b);
    return ret;
}

number number::operator^(const number b) const
{
    number ret;
    ret.value = value ^ b.value;
    return ret;
}

number number::operator&(const number b) const
{
    number ret;
    ret.value = value & b.value;
    return ret;
}

number number::operator|(const number b) const
{
    number ret;
    ret.value = value | b.value;
    return ret;
}

bool number::operator<(const number b) const
{
    bool ret;
    ret = value < b.value;
    return ret;
}

bool number::operator>(const number b) const
{
    return b < *this;
}

bool number::operator>=(const number b) const
{
    return !(*this < b);
}

bool number::operator<=(const number b) const
{
    return !(*this > b);
}

bool number::operator==(const number b) const
{
    return value == b.value;
}

std::string number::toBase(number base, const std::string *alp)
{
    logger log;
    if ((unsigned)base > 64 || (unsigned)base < 2)
        log.THROW("requesting convertion to non-existing base");
    std::string res;
    number me = *this;
    while ((unsigned)me > 0)
    {
        res.push_back(alp->at(me % base));
        me = me / base;
    }
    std::reverse(res.begin(), res.end());
    if (res.length() == 0)
        res = "0";
    return res;
}