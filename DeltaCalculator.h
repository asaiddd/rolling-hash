#pragma once

#include <vector>
#include <string>

class DeltaCalculator
{
private:
    struct Chunks
    {
        unsigned char *data;
        uint hash;
        long size;
        uint chunkCount;
        long startIdx;
        long endIdx;
        bool finished;
    };

    struct Chunks chunks1, chunks2;

    short chunkSize;
    std::vector<uint> additions;
    std::vector<uint> changes;
    std::vector<uint> removals;
    void initHashes();
    int iterate();
    void handleAdditions();
    void handleRemovals();

public:
    DeltaCalculator();
    ~DeltaCalculator();
    DeltaCalculator(unsigned char *data1, unsigned char *data2, const short chunkSize = 4);

    std::tuple<std::vector<uint>, std::vector<uint>, std::vector<uint>> calculateDelta();
};
