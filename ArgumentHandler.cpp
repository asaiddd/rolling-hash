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

std::tuple<std::string, std::string, std::string> ArgumentHandler::getFilenames()
{
    return {filename1, filename2, deltaFileName};
}

void ArgumentHandler::printHelp(std::string error)
{
    fprintf(stderr, "Error: %s\n", error.c_str());
    printf("Help: \n");
    printf("Usage: rollhashdiff -signatureFile -file2 -deltaFile\n");
    exit(EXIT_SUCCESS);
}