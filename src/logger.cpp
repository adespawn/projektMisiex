#include "logger.h"



void logger::THROW(std::string msg)
{
    if (msg.size() > 0 && msg.back() != '\n')
        msg.push_back('\n');
    std::cout << msg;
    exit(1);
}