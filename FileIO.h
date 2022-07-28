#pragma once

#include <stdio.h>
#include <string>
#include <vector>

class FileIO
{
private:
    static FILE *openReadOnlyFile(const std::string filename);

public:
    FileIO();
    ~FileIO();

    // returns a pointer to byte array on the heap
    static unsigned char *readFile(const std::string filename);
    static FILE *openWriteOnlyFile(const std::string filename);
    static void writeDelta(FILE *fp, std::vector<uint> delta, std::string deltaType);
};
