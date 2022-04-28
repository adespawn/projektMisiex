#include "string"
#include "logger.h"
#include "algorithm"

#ifndef __NUMBER_C
#define __NUMBER_C

class number
{
private:
    long long value;

public:
    number(long long);
    ~number();

    // Operators:
    // arithmetic:
    number operator+(const number) const;
    number operator-(const number) const;
    number operator*(const number) const;
    number operator/(const number) const;
    number operator^(const number) const;
    number operator&(const number) const;
    number operator|(const number) const;
    bool operator<(const number) const;
    bool operator>(const number) const;
    bool operator<=(const number) const;
    bool operator>=(const number) const;
    bool operator==(const number) const;

    // other:
    number operator=(const long long);

    long long val()
    {
        return value;
    }
    operator long long() const
    {
        return value;
    };

    std::string toBase(number, const std::string *);
};

#endif