#include "fstream"
#include "runtime.h"

int main(int argc, char **argv)
{
    logger log;
    if (argc < 2)
        log.THROW("Provide file name to run");
    try
    {
        std::ifstream file(argv[1]);
        std::string code;
        file >> code;
        runtime prog(code);
        prog.run();
    }
    catch (const std::exception &e)
    {
        log.THROW(e.what());
    }
}