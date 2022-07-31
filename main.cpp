#include <string.h>
#include <stdio.h>
#include <iostream>

#include "ArgumentHandler.h"
#include "FileIO.h"
#include "DeltaCalculator.h"

int main(int argc, char **argv)
{
    auto argHandler = ArgumentHandler(argc, argv);
    std::vector<std::string> filenames = argHandler.getFilenames();

    char *file1 = FileIO::readFile(filenames[0]);
    char *file2 = FileIO::readFile(filenames[1]);
    DeltaCalculator d1(file1, file2);
    std::vector<std::vector<uint>> delta = d1.calculateDelta();

    FILE *deltaFile = FileIO::openWriteOnlyFile(filenames[2]);

    FileIO::writeDelta(deltaFile, delta[0], "Additions");
    FileIO::writeDelta(deltaFile, delta[1], "Changes");
    FileIO::writeDelta(deltaFile, delta[2], "Removals");
        
    fclose(deltaFile);
    free(file1);
    free(file2);

    return 1;
}