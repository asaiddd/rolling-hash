#include "ArgumentHandler.h"
#include "FileIO.h"

#include "string.h"

ArgumentHandler::ArgumentHandler()
{
    filename1 = "";
    filename2 = "";
    deltaFileName = "";
}

ArgumentHandler::ArgumentHandler(int argc, char **argv)
{
    if (argc != PARAMETER_COUNT)
    {
        printHelp("Check number of parameters");
    }

    filename1 = argv[1];
    filename2 = argv[2];
    deltaFileName = argv[3];
}

ArgumentHandler::~ArgumentHandler() {}

std::vector<std::string> ArgumentHandler::getFilenames()
{
    return {std::move(filename1), std::move(filename2), std::move(deltaFileName)};
}

void ArgumentHandler::printHelp(std::string error)
{
    fprintf(stderr, "Error: %s\n", error.c_str());
    printf("Help: \n");
    printf("Usage: rollhashdiff -file1 -file2 -deltaFile\n");
    exit(EXIT_SUCCESS);
}
