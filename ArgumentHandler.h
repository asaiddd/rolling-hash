#pragma once

#include <string>
#include <vector>

class ArgumentHandler
{
private:
    const short PARAMETER_COUNT = 4;
    std::string filename1;
    std::string filename2;
    std::string deltaFileName;

    void handleDelta(int argc, char **argv);
    void printHelp(std::string error);

public:
    ArgumentHandler(int argc, char **argv);
    ~ArgumentHandler();
    ArgumentHandler();
    std::vector<std::string> getFilenames();
};
