#pragma once

#include <vector>
#include <string>

class DeltaCalculator
{
private:
    struct Chunks
    {
        char *data;
        uint hash;
        long size;
        uint chunkCount;
        long startIdx;
        long endIdx;
        bool lastChunk;
    };

    struct Chunks chunks1, chunks2;

    short chunkSize;
    std::vector<uint> additions;
    std::vector<uint> changes;
    std::vector<uint> removals;

    void initHashes();
    void iterate();
    void handleAdditions(long startIdx);
    void handleRemovals(long startIdx);
    void lastChunk();

public:
    DeltaCalculator();
    ~DeltaCalculator();
    DeltaCalculator(char *data1, char *data2, const short chunkSize = 4);
    std::vector<char*> getChunks();

    std::tuple<std::vector<uint>, std::vector<uint>, std::vector<uint>> calculateDelta();
};
