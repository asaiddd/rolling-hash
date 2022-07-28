#include <string.h>
#include <stdio.h>
#include <iostream>

#include "ArgumentHandler.h"
#include "FileIO.h"
#include "DeltaCalculator.h"

int main(int argc, char **argv)
{
    auto argHandler = ArgumentHandler(argc, argv);
    auto [filename1, filename2, deltaFilename] = argHandler.getFilenames();

    std::cout << filename1 << filename2 << deltaFilename << std::endl;

    unsigned char *file1 = FileIO::readFile(filename1);
    unsigned char *file2 = FileIO::readFile(filename2);
    DeltaCalculator d1(file1, file2);
    auto [additions, changes, removals] = d1.calculateDelta();

    FILE *deltaFile = FileIO::openWriteOnlyFile(deltaFilename);

    FileIO::writeDelta(deltaFile, additions, "Additions");
    FileIO::writeDelta(deltaFile, changes, "Changes");
    FileIO::writeDelta(deltaFile, removals, "Removals");
    return 1;
}