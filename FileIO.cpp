#include "FileIO.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

FileIO::FileIO() {}

FileIO::~FileIO() {}

FILE *FileIO::openReadOnlyFile(const std::string filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL)
    {
        fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fp;
}

FILE *FileIO::openWriteOnlyFile(const std::string filename)
{
    FILE *fp = fopen(filename.c_str(), "w");
    if (fp == NULL)
    {
        fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fp;
}

// returns a pointer to byte array on the heap
char *FileIO::readFile(const ::std::string filename)
{
    FILE *fp = openReadOnlyFile(filename);
    if (fseek(fp, 0, SEEK_END) != 0) // set cursor to end
    {
        fprintf(stderr, "fseek() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    long size = ftell(fp); // get file size
    if (size == 0)
    {
        printf("File is empty\n");
        exit(EXIT_SUCCESS);
    }
    else if (size == -1)
    {
        fprintf(stderr, "ftell() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    rewind(fp); // set cursor to start

    char *buffer = (char *)malloc(size);
    if (buffer == NULL)
    {
        fprintf(stderr, "malloc() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, size, 1, fp) <= 0)
    { // actually 0 is not an error but we do not expect it
        fprintf(stderr, "fread() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    return buffer;
}

void FileIO::writeDelta(FILE *fp, std::vector<uint> delta, std::string deltaType)
{
    if (!delta.empty())
    {
        fprintf(fp, "%s\n", deltaType.c_str());
        for (auto elem : delta)
        {
            fprintf(fp, "%d\n", elem);
        }
    }
}
